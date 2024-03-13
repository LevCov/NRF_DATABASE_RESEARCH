#include <libcouchbase/couchbase.h>
#include <libcouchbase/ixmgmt.h>

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

static void get_callback(lcb_INSTANCE *, int, const lcb_RESPGET *resp) {
  Result *result = nullptr;
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

int main() {
  std::string username{"Administrator"};
  std::string password{"000000"};
  std::string connection_string{"couchbase://127.0.0.1/test_bucket"};
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

  // Install the callback for GET operations. Note this can be done at any
  // time before the operation is scheduled
  lcb_install_callback(instance, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(get_callback));

  {
    Result result{};

    lcb_CMDGET *cmd = nullptr;
    check(lcb_cmdget_create(&cmd), "create GET command");
    check(lcb_cmdget_key(cmd, key.c_str(), key.size()),
          "assign ID for GET command");
    check(lcb_get(instance, &result, cmd), "schedule GET command");
    check(lcb_cmdget_destroy(cmd), "destroy GET command");
    lcb_wait(instance, LCB_WAIT_DEFAULT);

    std::cout << "Status for getting \"" << key << "\" is "
              << lcb_strerror_short(result.status)
              << ". Value: " << result.value << "\n";
  }

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
  std::string key_{"new_key"};
  std::string value_{R"({"some":"json"})"};
  cbi.create(instance, key_, value_);

  Result_ get_cbi{cbi.read(instance, key_)};

  std::cout << "Status for getting \"" << key_ << "\" is "
            << lcb_strerror_short(get_cbi.status)
            << ". Value: " << get_cbi.value << "\n";

  std::string value_n{R"({"some":"new_json"})"};
  cbi.update(instance, key_, value_n);

  cbi.del(instance, key_);

  cbi.createUniDB(
      "/Users/georgryabov/Desktop/main/wtf/NRF_DATABASE_RESEARCH/couchbase/"
      "data/data.json",
      10, instance);

  const char *bktname{"test_bucket"};
  lcb_cntl(instance, LCB_CNTL_GET, LCB_CNTL_BUCKETNAME, &bktname);

  lcb_CMDN1XMGMT cmd = {};
  cmd.spec.flags = LCB_N1XSPEC_F_PRIMARY;
  cmd.spec.keyspace = bktname;
  cmd.spec.nkeyspace = strlen(bktname);
  cmd.callback = ixmgmt_callback;
  lcb_n1x_create(instance, nullptr, &cmd);
  lcb_wait(instance, LCB_WAIT_DEFAULT);

  lcb_destroy(instance);
}