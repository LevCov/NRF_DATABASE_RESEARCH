#include "../include/couchbase.h"

#include <libcouchbase/couchbase.h>
#include <libcouchbase/ixmgmt.h>

#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "../../redisdb/include/nfTypes.h"

/**
 ** \brief Represents a interface and search interactions in the redis database.
 */

using json = nlohmann::json;

void query_callback(lcb_INSTANCE *, int, const lcb_RESPQUERY *resp) {
  lcb_STATUS status = lcb_respquery_status(resp);
  if (status != LCB_SUCCESS) {
    const lcb_QUERY_ERROR_CONTEXT *ctx;
    lcb_respquery_error_context(resp, &ctx);

    uint32_t err_code = 0;
    lcb_errctx_query_first_error_code(ctx, &err_code);

    const char *err_msg = nullptr;
    size_t err_msg_len = 0;
    lcb_errctx_query_first_error_message(ctx, &err_msg, &err_msg_len);
    std::string error_message{};
    if (err_msg_len > 0) {
      error_message.assign(err_msg, err_msg_len);
    }

    std::cerr << "[ERROR] failed to execute query. " << error_message << " ("
              << err_code << ")\n";
    return;
  }

  const char *buf = nullptr;
  std::size_t buf_len = 0;
  lcb_respquery_row(resp, &buf, &buf_len);
  if (buf_len > 0) {
    Rows *result = nullptr;
    lcb_respquery_cookie(resp, reinterpret_cast<void **>(&result));
    if (lcb_respquery_is_final(resp)) {
      result->metadata.assign(buf, buf_len);
    } else {
      result->rows.emplace_back(std::string(buf, buf_len));
    }
  }
}

void CouchBaseInterface::create(lcb_INSTANCE *instance, const std::string key,
                                const std::string value) {
  lcb_CMDSTORE *cmd = nullptr;
  check(lcb_cmdstore_create(&cmd, LCB_STORE_UPSERT), "create UPSERT command");
  check(lcb_cmdstore_key(cmd, key.c_str(), key.size()),
        "assign ID for UPSERT command");
  check(lcb_cmdstore_value(cmd, value.c_str(), value.size()),
        "assign value for UPSERT command");
  check(lcb_store(instance, nullptr, cmd), "schedule UPSERT command");
  check(lcb_cmdstore_destroy(cmd), "destroy UPSERT command");
  lcb_wait(instance, LCB_WAIT_DEFAULT);
}

// read by id.
Result_ CouchBaseInterface::search(lcb_INSTANCE *instance,
                                   const std::string key, Result_ &result) {
  lcb_CMDGET *cmd = nullptr;
  check(lcb_cmdget_create(&cmd), "create GET command");
  check(lcb_cmdget_key(cmd, key.c_str(), key.size()),
        "assign ID for GET command");
  check(lcb_get(instance, &result, cmd), "schedule GET command");
  check(lcb_cmdget_destroy(cmd), "destroy GET command");
  lcb_wait(instance, LCB_WAIT_DEFAULT);

  return result;
}

void CouchBaseInterface::update(lcb_INSTANCE *instance, const std::string key,
                                const std::string value) {
  Result_u result{};
  lcb_CMDSTORE *cmd = nullptr;
  check(lcb_cmdstore_create(&cmd, LCB_STORE_REPLACE), "create REPLACE command");
  check(lcb_cmdstore_key(cmd, key.c_str(), key.size()),
        "assign ID for REPLACE command");
  check(lcb_cmdstore_value(cmd, value.c_str(), value.size()),
        "assign value for REPLACE command");
  check(lcb_store(instance, &result, cmd), "schedule REPLACE command");
  check(lcb_cmdstore_destroy(cmd), "destroy UPSERT command");
  lcb_wait(instance, LCB_WAIT_DEFAULT);

  if (result.rc != LCB_SUCCESS) {
    std::cout << "failed to append " << ": " << lcb_strerror_short(result.rc)
              << "\n";
  }
}

void CouchBaseInterface::del(lcb_INSTANCE *instance, const std::string key) {
  lcb_CMDREMOVE *cmd = nullptr;
  check(lcb_cmdremove_create(&cmd), "create REMOVE command");
  check(lcb_cmdremove_key(cmd, key.c_str(), key.size()),
        "assign key for REMOVE command");
  check(lcb_remove(instance, nullptr, cmd), "schedule REMOVE command");
  check(lcb_cmdremove_destroy(cmd), "destroy REMOVE command");
  lcb_wait(instance, LCB_WAIT_DEFAULT);
}

static void ixmgmt_callback(__unused lcb_INSTANCE *instance,
                            __unused int cbtype,
                            const struct lcb_RESPN1XMGMT_st *resp);

void CouchBaseInterface::createUniDB(const char *config_path, int n,
                                     lcb_INSTANCE *instance) {
  const char *bktname{"test_bucket"};

  lcb_cntl(instance, LCB_CNTL_GET, LCB_CNTL_BUCKETNAME, &bktname);

  lcb_CMDN1XMGMT cmd = {};
  cmd.spec.flags = LCB_N1XSPEC_F_PRIMARY;
  cmd.spec.keyspace = bktname;
  cmd.spec.nkeyspace = strlen(bktname);
  cmd.spec.name = "nfInstanceId";
  cmd.spec.nname = 12;
  cmd.callback = ixmgmt_callback;
  lcb_n1x_create(instance, nullptr, &cmd);
  lcb_wait(instance, LCB_WAIT_DEFAULT);

  std::ifstream cf(config_path);
  if (cf.is_open()) {
    std::mt19937 rng;
    const unsigned int seed = 42;
    rng.seed(seed);
    std::uniform_int_distribution<int> dist(0, 21);

    json instances = json::parse(cf);
    for (auto &profile : instances) {  // for 1 of 1.
      for (int i = 0; i < n; ++i) {
        const std::string nfInstanceId = std::to_string(i);
        const int randomNumber = dist(rng);
        profile["nfInstanceId"] = nfInstanceId;
        profile["nfType"] = nfTypes[randomNumber];
        (*this).create(instance, nfInstanceId, profile.dump());
      }
    }
    cf.close();
  }
}

void CouchBaseInterface::read(lcb_INSTANCE *instance,
                              const std::string &bucketName,
                              const std::string &nfInstance, Rows &result) {
  // tag::query[]
  std::string statement = "SELECT * FROM `" + bucketName +
                          R"(` WHERE nfType=")" + nfInstance + R"(")";

  lcb_CMDQUERY *cmd = nullptr;
  CouchBaseInterface::check(lcb_cmdquery_create(&cmd), "create QUERY command");
  CouchBaseInterface::check(
      lcb_cmdquery_statement(cmd, statement.c_str(), statement.size()),
      "assign statement for QUERY command");
  CouchBaseInterface::check(lcb_cmdquery_callback(cmd, query_callback),
                            "assign callback for QUERY command");
  CouchBaseInterface::check(lcb_query(instance, &result, cmd),
                            "schedule QUERY command");
  CouchBaseInterface::check(lcb_cmdquery_destroy(cmd), "destroy QUERY command");
  lcb_wait(instance, LCB_WAIT_DEFAULT);
  // end::query[]
  // std::cout << "Query returned " << result.rows.size() << " rows\n";
  // for (const auto &row : result.rows) {
  //   std::cout << row << "\n";
  // }
  // std::cout << "\nMetadata:\n" << result.metadata << "\n";
}

//
// callbacks block
//
static void ixmgmt_callback(__unused lcb_INSTANCE *instance,
                            __unused int cbtype,
                            const struct lcb_RESPN1XMGMT_st *resp) {
  if (resp->rc == LCB_SUCCESS) {
    std::cout << "Index was successfully created!" << std::endl;
  } else if (resp->rc == LCB_ERR_INDEX_EXISTS) {
    std::cout << "Index already exists!!" << std::endl;
  } else {
    std::cout << "Operation failed: " << lcb_strerror_long(resp->rc)
              << std::endl;
  }
}
