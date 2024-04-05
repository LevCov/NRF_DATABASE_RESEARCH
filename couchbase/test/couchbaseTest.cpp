//===----------------------------------------------------------------------===//

// #include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <hiredis/hiredis.h>
#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

// #include "../../include/nfTypes.h"
#include "../include/couchbase.h"

using CBI = CouchBaseInterface;

static void get_callback(lcb_INSTANCE *, int, const lcb_RESPGET *resp) {
  Result_ *result = nullptr;
  lcb_respget_cookie(resp, reinterpret_cast<void **>(&result));

  result->status = lcb_respget_status(resp);
  result->value.clear();  // Remove any prior value
  if (result->status == LCB_SUCCESS) {
    const char *buf = nullptr;
    std::size_t buf_len = 0;
    CBI::check(lcb_respget_value(resp, &buf, &buf_len),
               "extract value from GET response");
    result->value.assign(buf, buf_len);
  }
}

static lcb_INSTANCE *init() {
  std::string username{"Administrator"};
  std::string password{"000000"};
  std::string connection_string{"couchbase://127.0.0.1"};
  std::string bucket_name{"test_bucket"};

  lcb_CREATEOPTS *create_options = nullptr;
  CBI::check(lcb_createopts_create(&create_options, LCB_TYPE_BUCKET),
             "build options object for lcb_create");
  CBI::check(lcb_createopts_credentials(create_options, username.c_str(),
                                        username.size(), password.c_str(),
                                        password.size()),
             "assign credentials");
  CBI::check(lcb_createopts_connstr(create_options, connection_string.c_str(),
                                    connection_string.size()),
             "assign connection string");
  CBI::check(lcb_createopts_bucket(create_options, bucket_name.c_str(),
                                   bucket_name.size()),
             "assign bucket name");

  lcb_INSTANCE *instance = nullptr;
  CBI::check(lcb_create(&instance, create_options), "create lcb_INSTANCE");
  CBI::check(lcb_createopts_destroy(create_options), "destroy options object");
  CBI::check(lcb_connect(instance), "schedule connection");
  CBI::check(lcb_wait(instance, LCB_WAIT_DEFAULT), "wait for connection");
  CBI::check(lcb_get_bootstrap_status(instance), "check bootstrap status");
  return instance;
}

CBI database;
//===----------------------------------------------------------------------===//
// CRUD redefined from the redis++ library.
//===----------------------------------------------------------------------===//

TEST(CRUD, create) {
  auto instance = init();
  lcb_install_callback(instance, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(get_callback));
  database.del(instance, "0");
  std::string key{"0"};
  std::string value{R"({"some":"json"})"};
  database.create(instance, key, value);

  Result_ result;
  lcb_CMDGET *cmd = nullptr;
  CBI::check(lcb_cmdget_create(&cmd), "create GET command");
  CBI::check(lcb_cmdget_key(cmd, key.c_str(), key.size()),
             "assign ID for GET command");
  CBI::check(lcb_get(instance, &result, cmd), "schedule GET command");
  CBI::check(lcb_cmdget_destroy(cmd), "destroy GET command");
  lcb_wait(instance, LCB_WAIT_DEFAULT);

  bool flag = value == result.value;
  EXPECT_EQ(flag, true);
  database.del(instance, "0");
}

TEST(CRUD, search) {
  auto instance = init();
  lcb_install_callback(instance, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(get_callback));
  database.del(instance, "0");

  std::string key_{"0"};
  std::string value{R"({"some":"json"})"};
  database.create(instance, key_, value);

  Result_ get_cbi;
  database.search(instance, key_, get_cbi);

  bool flag = value == get_cbi.value;
  EXPECT_EQ(flag, true);
  database.del(instance, "0");
}

// TEST(CRUD, read) {
//   auto instance = init();
//   database.createUniDB(
//       "/Users/georgryabov/Desktop/main/wtf/NRF_DATABASE_RESEARCH/couchbase/"
//       "data/test.json",
//       1, instance);

//   Rows result{};
//   std::string bucketName{"test_bucket"};
//   std::string nfInstanceId{"0"};
//   database.read(instance, bucketName, nfInstanceId, result);

//   bool flag = false;
//   for (const auto &row : result.rows) {
//     flag =
//         row ==
//         R"({"test_bucket":{"nfInstanceId":"0","nfInstanceName":"string","nfType":"PCF"}})";
//   }

//   EXPECT_EQ(flag, true);
//   database.del(instance, nfInstanceId);
// }

TEST(CRUD, update) {
  auto instance = init();
  lcb_install_callback(instance, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(get_callback));
  std::string value{R"({"nfInstanceId":"0"})"};
  database.create(instance, "someKey", value);

  std::string value_n{R"({"some":"new_json"})"};
  database.update(instance, "someKey", value_n);
  Result_ get_cbi;

  database.search(instance, "someKey", get_cbi);
  bool flag = get_cbi.value == R"({"some":"new_json"})";
  EXPECT_EQ(flag, true);
}

TEST(CRUD, del) {
  auto instance = init();
  lcb_install_callback(instance, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(get_callback));
  std::string value{R"({"nfInstanceId":"0"})"};
  database.create(instance, "someKey", value);
  database.del(instance, "someKey");

  Result_ get_cbi;
  database.search(instance, "someKey", get_cbi);
  bool flag = get_cbi.status != LCB_SUCCESS;

  EXPECT_EQ(flag, true);
}

TEST(selfMethod, CreateUniDB) {
  auto instance = init();
  lcb_install_callback(instance, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(get_callback));
  std::string value{R"({"nfInstanceId":"0"})"};
  database.createUniDB(
      "/Users/georgryabov/Desktop/main/wtf/NRF_DATABASE_RESEARCH/couchbase/"
      "data/test.json",
      5, instance);
  const std::vector<std::string> ans{
      R"({"test_bucket":{"nfInstanceId":"0","nfInstanceName":"string","nfType":"PCF"}})",
      R"({"test_bucket":{"nfInstanceId":"1","nfInstanceName":"string","nfType":"CHF"}})",
      R"({"test_bucket":{"nfInstanceId":"2","nfInstanceName":"string","nfType":"UPF"}})",
      R"({"test_bucket":{"nfInstanceId":"3","nfInstanceName":"string","nfType":"LMF"}})",
      R"({"test_bucket":{"nfInstanceId":"4","nfInstanceName":"string","nfType":"SMSF"}})"};
  std::string bucketName{"test_bucket"};
  bool flag = true;
  for (size_t i = 0; i < 5; ++i) {
    Rows result{};
    std::string nfInstanceId{std::to_string(i)};
    database.read(instance, bucketName, nfInstanceId, result);

    for (const auto &row : result.rows) {
      flag = flag && (row == ans[i]);
    }
  }
  EXPECT_EQ(flag, true);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}