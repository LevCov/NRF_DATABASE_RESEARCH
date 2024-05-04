#include <gtest/gtest.h>

#include <filesystem>

#include "../include/couchbase.h"

auto database = std::make_unique<CouchBaseInterface>();

TEST(CRUD, create) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  const std::string_view key{"0"}, value{R"({"some":"json"})"};
  database->create(std::make_pair(key, value));
  lcb_CMDGET *cmd = nullptr;
  check(lcb_cmdget_create(&cmd), "create GET command");
  check(lcb_cmdget_key(cmd, key.data(), key.size()),
        "assign ID for GET command");
  check(lcb_get(database->instance_, &database->result_, cmd),
        "schedule GET command");
  check(lcb_cmdget_destroy(cmd), "destroy GET command");
  lcb_wait(database->instance_, LCB_WAIT_DEFAULT);
  bool flag = value == database->result_.value;
  EXPECT_EQ(flag, true);
}

TEST(CRUD, read) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  const std::string_view key{"0"}, value{R"({"some":"json"})"};
  database->create(std::make_pair(key, value));
  database->read(key);
  bool flag = value == database->result_.value;
  EXPECT_EQ(flag, true);
}

TEST(CRUD, find) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  database->createUniDB(
      "/Users/georgryabov/Desktop/main/wtf/NRF_DATABASE_RESEARCH/interface/"
      "couchbase/data/test.json",
      2);
  const std::string_view bucketName{"test_bucket"}, nfType{"CHF"};
  database->find(std::make_pair(bucketName, nfType));
  bool flag = false;
  for (const auto &row : database->resultRows_.rows) {
    if (row ==
        R"({"test_bucket":{"nfInstanceId":"1","nfInstanceName":"string","nfType":"CHF"}})")
      flag = true;
  }
  EXPECT_EQ(flag, true);
}

TEST(CRUD, update) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  const std::string_view value{R"({"nfInstanceId":"0"})"};
  database->create(std::make_pair("someKey", value));
  const std::string_view value_n{R"({"some":"new_json"})"};
  database->update("someKey", value_n);
  database->read("someKey");
  bool flag = database->result_.value == R"({"some":"new_json"})";
  EXPECT_EQ(flag, true);
}

TEST(CRUD, del) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  const std::string_view value{R"({"nfInstanceId":"0"})"};
  database->create(std::make_pair("someKey", value));
  database->del("someKey");
  database->read("someKey");
  bool flag = database->result_.status != LCB_SUCCESS;
  EXPECT_EQ(flag, true);
}

TEST(selfMethod, CreateUniDB) {
  auto database = std::make_unique<CouchBaseInterface>();
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  database->createUniDB(
      "/Users/georgryabov/Desktop/main/wtf/NRF_DATABASE_RESEARCH/interface/"
      "couchbase/data/test.json",
      5);
  const std::vector<std::string_view> ans{
      R"({"test_bucket":{"nfInstanceId":"0","nfInstanceName":"string","nfType":"PCF"}})",
      R"({"test_bucket":{"nfInstanceId":"1","nfInstanceName":"string","nfType":"CHF"}})",
      R"({"test_bucket":{"nfInstanceId":"2","nfInstanceName":"string","nfType":"UPF"}})",
      R"({"test_bucket":{"nfInstanceId":"3","nfInstanceName":"string","nfType":"LMF"}})",
      R"({"test_bucket":{"nfInstanceId":"4","nfInstanceName":"string","nfType":"SMSF"}})"};
  const std::vector<std::string_view> ansTypes{"PCF", "CHF", "UPF", "LMF",
                                               "SMSF"};
  std::string_view bucketName{"test_bucket"};
  bool flag = true;
  for (int i = 0; i < 5; ++i) {
    database->find(std::make_pair(bucketName, ansTypes[i]));
    for (const auto &row : database->resultRows_.rows) {
      flag = flag && (row == ans[i]);
    }
    database->resultRows_ = {};
  }
  EXPECT_EQ(flag, true);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}