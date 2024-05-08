#include "../include/couchbase.h"

#include <libcouchbase/couchbase.h>
#include <libcouchbase/ixmgmt.h>

using json = nlohmann::json;

/**
 ** \brief Represents a interface and search interactions in the redis database.
 */

CouchBaseInterface::CouchBaseInterface() {
  const std::string_view username{"Administrator"}, password{"000000"},
      connection_string{"couchbase://127.0.0.1"}, bucket_name{"test_bucket"};
  lcb_CREATEOPTS *create_options = nullptr;
  check(lcb_createopts_create(&create_options, LCB_TYPE_BUCKET),
        "build options object for lcb_create");
  check(lcb_createopts_credentials(create_options, username.data(),
                                   username.size(), password.data(),
                                   password.size()),
        "assign credentials");
  check(lcb_createopts_connstr(create_options, connection_string.data(),
                               connection_string.size()),
        "assign connection string");
  check(lcb_createopts_bucket(create_options, bucket_name.data(),
                              bucket_name.size()),
        "assign bucket name");
  check(lcb_create(&instance_, create_options), "create lcb_INSTANCE");
  check(lcb_createopts_destroy(create_options), "destroy options object");
  check(lcb_connect(instance_), "schedule connection");
  check(lcb_wait(instance_, LCB_WAIT_DEFAULT), "wait for connection");
  check(lcb_get_bootstrap_status(instance_), "check bootstrap status");
}

bool CouchBaseInterface::create(
    const std::pair<std::string_view, std::string_view> &var) {
  lcb_wait(instance_, LCB_WAIT_DEFAULT);
  lcb_CMDSTORE *cmd = nullptr;
  check(lcb_cmdstore_create(&cmd, LCB_STORE_UPSERT), "create UPSERT command");
  check(lcb_cmdstore_key(cmd, var.first.data(), var.first.size()),
        "assign ID for UPSERT command");
  check(lcb_cmdstore_value(cmd, var.second.data(), var.second.size()),
        "assign value for UPSERT command");
  check(lcb_store(instance_, nullptr, cmd), "schedule UPSERT command");
  check(lcb_cmdstore_destroy(cmd), "destroy UPSERT command");
  lcb_wait(instance_, LCB_WAIT_DEFAULT);
  return true;
}

void CouchBaseInterface::read(const std::string_view &key) {
  lcb_CMDGET *cmd = nullptr;
  check(lcb_cmdget_create(&cmd), "create GET command");
  check(lcb_cmdget_key(cmd, key.data(), key.size()),
        "assign ID for GET command");
  check(lcb_get(instance_, &result_, cmd), "schedule GET command");
  check(lcb_cmdget_destroy(cmd), "destroy GET command");
  lcb_wait(instance_, LCB_WAIT_DEFAULT);
}

bool CouchBaseInterface::update(const std::string_view &key,
                                const std::string_view &value) {
  Result_u result{};
  lcb_CMDSTORE *cmd = nullptr;
  check(lcb_cmdstore_create(&cmd, LCB_STORE_REPLACE), "create REPLACE command");
  check(lcb_cmdstore_key(cmd, key.data(), key.size()),
        "assign ID for REPLACE command");
  check(lcb_cmdstore_value(cmd, value.data(), value.size()),
        "assign value for REPLACE command");
  check(lcb_store(instance_, &result, cmd), "schedule REPLACE command");
  check(lcb_cmdstore_destroy(cmd), "destroy UPSERT command");
  lcb_wait(instance_, LCB_WAIT_DEFAULT);
  if (result.rc != LCB_SUCCESS) {
    std::cout << "failed to append " << ": " << lcb_strerror_short(result.rc)
              << "\n";
    return false;
  }
  return true;
}

void CouchBaseInterface::del(const std::string_view &key) {
  lcb_CMDREMOVE *cmd = nullptr;
  check(lcb_cmdremove_create(&cmd), "create REMOVE command");
  check(lcb_cmdremove_key(cmd, key.data(), key.size()),
        "assign key for REMOVE command");
  check(lcb_remove(instance_, nullptr, cmd), "schedule REMOVE command");
  check(lcb_cmdremove_destroy(cmd), "destroy REMOVE command");
  lcb_wait(instance_, LCB_WAIT_DEFAULT);
}

void CouchBaseInterface::createUniDB(const char *config_path, const size_t n) {
  lcb_wait(instance_, LCB_WAIT_DEFAULT);
  // const char *bktname{"test_bucket"};
  std::string_view bktname{"test_bucket"};
  lcb_cntl(instance_, LCB_CNTL_GET, LCB_CNTL_BUCKETNAME, &bktname);
  lcb_CMDN1XMGMT cmd = {};
  cmd.spec.flags = LCB_N1XSPEC_F_PRIMARY;
  cmd.spec.keyspace = bktname.data();
  cmd.spec.nkeyspace = strlen(bktname.data());
  cmd.callback = ixmgmtCallback;
  lcb_n1x_create(instance_, nullptr, &cmd);
  lcb_wait(instance_, LCB_WAIT_DEFAULT);
  std::ifstream cf(config_path);
  if (cf.is_open()) {
    std::mt19937 rng;
    const uint8_t seed = 42;
    rng.seed(seed);
    std::uniform_int_distribution<int> dist(0, nfTypes.size());
    json instances = json::parse(cf);
    for (auto &&profile : instances) {  // for 1 of 1.
      for (size_t i = 0; i < n; ++i) {
        const std::string nfInstanceId = std::to_string(i);
        const int randomNumber = dist(rng);
        profile["nfInstanceId"] = nfInstanceId;
        profile["nfType"] = nfTypes[randomNumber];
        this->create(std::make_pair(nfInstanceId, profile.dump()));
        lcb_wait(instance_, LCB_WAIT_DEFAULT);
      }
    }
    cf.close();
  }
}

void CouchBaseInterface::find(
    const std::pair<std::string_view, std::string_view> &var) {
  std::string statement = "SELECT * FROM `" + std::string(var.first) +
                          R"(` WHERE nfType=")" + std::string(var.second) +
                          R"(")";
  lcb_CMDQUERY *cmd = nullptr;
  check(lcb_cmdquery_create(&cmd), "create QUERY command");
  check(lcb_cmdquery_statement(cmd, statement.c_str(), statement.size()),
        "assign statement for QUERY command");
  check(lcb_cmdquery_callback(cmd, queryCallback),
        "assign callback for QUERY command");
  check(lcb_query(instance_, &resultRows_, cmd), "schedule QUERY command");
  check(lcb_cmdquery_destroy(cmd), "destroy QUERY command");
  lcb_wait(instance_, LCB_WAIT_DEFAULT);
}

void CouchBaseInterface::flushdb() {
  const std::string flush_path{
      "/pools/default/buckets/test_bucket/controller/doFlush"};
  lcb_install_callback(instance_, LCB_CALLBACK_HTTP,
                       reinterpret_cast<lcb_RESPCALLBACK>(flushCallback));
  lcb_CMDHTTP *cmd = nullptr;
  check(lcb_cmdhttp_create(&cmd, LCB_HTTP_TYPE_MANAGEMENT),
        "create HTTP command object of MANAGEMENT type");
  check(lcb_cmdhttp_method(cmd, LCB_HTTP_METHOD_POST), "set HTTP method");
  check(lcb_cmdhttp_path(cmd, flush_path.c_str(), flush_path.size()),
        "set HTTP path");
  check(lcb_http(instance_, nullptr, cmd), "schedule HTTP command");
  check(lcb_cmdhttp_destroy(cmd), "destroy command object");
  check(lcb_wait(instance_, LCB_WAIT_DEFAULT), "wait for completion");
}

void ixmgmtCallback(__unused lcb_INSTANCE *instance_, __unused int cbtype,
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

void getCallback(lcb_INSTANCE *, int, const lcb_RESPGET *resp) {
  Result_ *result = nullptr;
  lcb_respget_cookie(resp, reinterpret_cast<void **>(&result));
  result->status = lcb_respget_status(resp);
  result->value.clear();  // Remove any prior value
  if (result->status == LCB_SUCCESS) {
    const char *buf = nullptr;
    std::size_t buf_len = 0;
    check(lcb_respget_value(resp, &buf, &buf_len),
          "extract value from GET response");
    result->value.assign(buf, buf_len);
  }
}

void queryCallback(lcb_INSTANCE *, int, const lcb_RESPQUERY *resp) {
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

void flushCallback(lcb_INSTANCE *, int, const lcb_RESPHTTP *resp) {
  check(lcb_resphttp_status(resp), "HTTP operation status in the callback");
  uint16_t status;
  lcb_resphttp_http_status(resp, &status);
  const char *buf;
  size_t buf_len = 0;
  lcb_resphttp_body(resp, &buf, &buf_len);
  if (buf_len > 0) {
    std::string body(buf, buf_len);
    if (body.find("Flush is disabled") != std::string::npos) {
      std::cerr << "[ERROR] Flush is not enabled for the specified bucket\n";
      return;
    }
  }
}

void check(lcb_STATUS err, const char *msg) {
  if (err != LCB_SUCCESS) {
    std::cerr << "[ERROR] " << msg << ": " << lcb_strerror_short(err) << "\n";
    exit(EXIT_FAILURE);
  }
}
