#include <libcouchbase/couchbase.h>
#include <libcouchbase/ixmgmt.h>

#include <cstdlib>
#include <iostream>
#include <string>

#include "../include/couchbase.h"

static void check(lcb_STATUS err, const char *msg) {
  if (err != LCB_SUCCESS) {
    std::cerr << "[ERROR] " << msg << ": " << lcb_strerror_short(err) << "\n";
    exit(EXIT_FAILURE);
  }
}

static void open_callback(lcb_INSTANCE *, lcb_STATUS rc) {
  check(rc, "opening the bucket");
  std::cout << "The bucket has been opened successfully\n";
}

struct Result {
  std::string value{};
  lcb_STATUS status{LCB_SUCCESS};
};

static void ixmgmt_callback(__unused lcb_INSTANCE *instance,
                            __unused int cbtype,
                            const struct lcb_RESPN1XMGMT_st *resp) {
  if (resp->rc == LCB_SUCCESS) {
    std::cout << "Index was successfully created!" << std::endl;
  } else if (resp->rc == LCB_ERR_INDEX_EXISTS) {
    std::cout << "Index already exists!" << std::endl;
  } else {
    std::cout << "Operation failed: " << lcb_strerror_long(resp->rc)
              << std::endl;
  }
}

static void row_callback(lcb_INSTANCE *instance, int type,
                         const lcb_RESPQUERY *resp) {
  int *idx = NULL;
  size_t row_len = 0;
  char *row = NULL;

  lcb_STATUS rc = lcb_respquery_status(resp);
  if (rc != LCB_SUCCESS) {
    printf("failed to execute query: %s\n", lcb_strerror_short(rc));
    exit(EXIT_FAILURE);
  }
  lcb_respquery_cookie(resp, (void **)(&idx));
  if (lcb_respquery_is_final(resp)) {
    printf("META: ");
  } else {
    printf("ROW #%d: ", (*idx)++);
  }
  // lcb_respquery_row(resp, &row, &row_len);
  printf("%.*s\n", row_len, row);
}

static void query_callback(lcb_INSTANCE *, int, const lcb_RESPQUERY *resp) {
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

int main() {
  std::string username{"Administrator"};
  std::string password{"000000"};
  std::string connection_string{"couchbase://127.0.0.1"};
  std::string bucket_name{"test_bucket"};

  lcb_CREATEOPTS *create_options = nullptr;
  check(lcb_createopts_create(&create_options, LCB_TYPE_BUCKET),
        "build options object for lcb_create");
  check(lcb_createopts_credentials(create_options, username.c_str(),
                                   username.size(), password.c_str(),
                                   password.size()),
        "assign credentials");
  check(lcb_createopts_connstr(create_options, connection_string.c_str(),
                               connection_string.size()),
        "assign connection string");
  check(lcb_createopts_bucket(create_options, bucket_name.c_str(),
                              bucket_name.size()),
        "assign bucket name");

  lcb_INSTANCE *instance = nullptr;
  check(lcb_create(&instance, create_options), "create lcb_INSTANCE");
  check(lcb_createopts_destroy(create_options), "destroy options object");
  check(lcb_connect(instance), "schedule connection");
  check(lcb_wait(instance, LCB_WAIT_DEFAULT), "wait for connection");
  check(lcb_get_bootstrap_status(instance), "check bootstrap status");

  // Store a key first, so we know it will exist later on. In real production
  // environments, we'd also want to install a callback for storage operations
  // so we know if they succeeded
  std::string key{"a_key"};

  {
    std::string value{R"({"some":"json"})"};

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

  // // Install the callback for GET operations. Note this can be done at any
  // // time before the operation is scheduled
  // lcb_install_callback(instance, LCB_CALLBACK_GET,
  //                      reinterpret_cast<lcb_RESPCALLBACK>(getCallback));

  // {
  //   Result result{};

  //   lcb_CMDGET *cmd = nullptr;
  //   check(lcb_cmdget_create(&cmd), "create GET command");
  //   check(lcb_cmdget_key(cmd, key.c_str(), key.size()),
  //         "assign ID for GET command");
  //   check(lcb_get(instance, &result, cmd), "schedule GET command");
  //   check(lcb_cmdget_destroy(cmd), "destroy GET command");
  //   lcb_wait(instance, LCB_WAIT_DEFAULT);

  //   std::cout << "Status for getting \"" << key << "\" is "
  //             << lcb_strerror_short(result.status) << ". Value: " <<
  //             result.value
  //             << "\n";
  // }

  // Let's see what happens if we get a key that isn't yet stored:
  {
    std::string non_existing{"non-existing-key"};

    Result result{};

    lcb_CMDGET *cmd = nullptr;
    check(lcb_cmdget_create(&cmd), "create GET command");
    check(lcb_cmdget_key(cmd, non_existing.c_str(), non_existing.size()),
          "assign ID for GET command");
    check(lcb_get(instance, &result, cmd), "schedule GET command");
    check(lcb_cmdget_destroy(cmd), "destroy GET command");
    lcb_wait(instance, LCB_WAIT_DEFAULT);

    std::cout << "Status for getting \"" << key << "\" is "
              << lcb_strerror_short(result.status) << ".\n";
  }

  CouchBaseInterface cbi;
  // std::string key_{"new_key"};
  // std::string value_{R"({"some":"json"})"};
  // cbi.create(instance, key_, value_);

  // Result_ get_cbi;
  // cbi.read(instance, key_, get_cbi);
  // std::cout << "\n\n"
  //           << "THIS!!\n";
  // std::cout << "Status for getting \"" << key_ << "\" is "
  //           << lcb_strerror_short(get_cbi.status)
  //           << ". Value: " << get_cbi.value << "\n";

  // std::string value_n{R"({"some":"new_json"})"};
  // cbi.update(instance, key_, value_n);

  // cbi.del(instance, key_);

  // const char *bktname{"test_bucket"};
  // {
  //   lcb_cntl(instance, LCB_CNTL_GET, LCB_CNTL_BUCKETNAME, &bktname);

  //   lcb_CMDN1XMGMT cmd = {};
  //   cmd.spec.flags = LCB_N1XSPEC_F_PRIMARY;
  //   cmd.spec.keyspace = bktname;
  //   cmd.spec.nkeyspace = strlen(bktname);
  //   cmd.spec.name = "nfInstanceId";
  //   cmd.spec.nname = 12;
  //   cmd.callback = ixmgmt_callback;
  //   lcb_n1x_create(instance, nullptr, &cmd);
  //   lcb_wait(instance, LCB_WAIT_DEFAULT);
  // }

  // {
  //   Rows result{};

  //   // tag::query[]
  //   std::cout << "bucket_name = " << bucket_name << std::endl;
  //   std::string statement =
  //       "SELECT * FROM `" + bucket_name + R"(` WHERE nfInstanceId="0")";

  //   lcb_CMDQUERY *cmd = nullptr;
  //   check(lcb_cmdquery_create(&cmd), "create QUERY command");
  //   check(lcb_cmdquery_statement(cmd, statement.c_str(), statement.size()),
  //         "assign statement for QUERY command");
  //   check(lcb_cmdquery_callback(cmd, query_callback),
  //         "assign callback for QUERY command");
  //   check(lcb_query(instance, &result, cmd), "schedule QUERY command");
  //   check(lcb_cmdquery_destroy(cmd), "destroy QUERY command");
  //   lcb_wait(instance, LCB_WAIT_DEFAULT);
  //   // end::query[]

  //   std::cout << "Query returned " << result.rows.size() << " rows\n";
  //   for (const auto &row : result.rows) {
  //     std::cout << row << "\n";
  //   }
  //   // std::cout << "\nMetadata:\n" << result.metadata << "\n";
  // }
  std::string bucketName{"test_bucket"};
  std::string nfInstanceId{"0"};
  std::cout << "\n\n??????\n\n";
  cbi.read(nfInstanceId);

  std::cout << "\n\nSLKANDKAMSOKDMAPSKMDPASMDPMAS\n\n";
  cbi.flushdb();

  cbi.createUniDB(
      "/Users/georgryabov/Desktop/main/wtf/NRF_DATABASE_RESEARCH/couchbase/"
      "data/test.json",
      100);

  // const std::string bucketName{"test_bucket"};

  cbi.find(std::make_pair(bucketName, "PCF"));
  std::cout << "\n" << cbi.resultRows_.rows.size() << "\n";

  lcb_destroy(instance);
}