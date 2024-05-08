//===----------------------------------------------------------------------===//

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include "../../RedisDB/include/RedisDBInterface.h"
#include "../../RedisDB/nfTypes.h"
#include "../../UUID/UUIDv4.h"

const std::string path_dir = std::filesystem::current_path().remove_filename();
RedisDBInterface redis("tcp://127.0.0.1:6379");

namespace {
using json = nlohmann::json;
using keys_t = std::vector<OptionalString>;
}  // namespace

//===----------------------------------------------------------------------===//
// CRUD redefined from the redis++ library.
//===----------------------------------------------------------------------===//

TEST(CRUD, create) {
  checkRedisConnection(redis.connection_);
  redis.connection_->flushdb();
  bool flag = true;
  CreateRedis var("test_create_key", "nfType", "test_create_val");
  flag *= redis.create(var);
  flag *=
      redis.connection_->hget("test_create_key", "nfType") == "test_create_val";
  EXPECT_EQ(flag, true);
  redis.connection_->flushdb();
}

TEST(CRUD, read) {
  checkRedisConnection(redis.connection_);
  redis.connection_->flushdb();
  bool flag = true;
  CreateRedis varFirst("test_create_key", "nfType", "test_create_val");
  flag *= redis.create(varFirst);
  CreateRedis varSecond("test_create_key", "dataField", "someData");
  flag *= redis.create(varSecond);
  flag *= redis.read(std::make_pair("test_create_key", "nfType")) ==
          "test_create_val";
  flag *=
      redis.read(std::make_pair("test_create_key", "dataField")) == "someData";
  EXPECT_EQ(flag, true);
  redis.connection_->flushdb();
}

// RedisInterface::update with exist key.
TEST(CRUD, update_exist) {
  checkRedisConnection(redis.connection_);
  redis.connection_->flushdb();
  bool flag = true;
  CreateRedis varFirst("test_create_key", "nfType", "test_create_val");
  flag *= redis.create(varFirst);
  CreateRedis varSecond("test_create_key", "dataField", "someData");
  flag *= redis.create(varSecond);

  flag *= redis.update(std::make_pair("test_create_key", "nfType"),
                       std::make_pair("test_create_val_new", ""));
  flag *= redis.update(std::make_pair("test_create_key", "dataField"),
                       std::make_pair("someData_new", ""));
  EXPECT_EQ(flag, false);
  redis.connection_->flushdb();
}

// RedisInterface::update with no exist key.
TEST(CRUD, update_no_exist) {
  checkRedisConnection(redis.connection_);
  redis.connection_->flushdb();
  bool flag = true;
  flag *= redis.update(std::make_pair("test_create_key", "nfType"),
                       std::make_pair("test_create_val_new", ""));
  flag *= redis.update(std::make_pair("test_create_key", "dataField"),
                       std::make_pair("someData_new", ""));
  flag *= redis.read(std::make_pair("test_create_key", "nfType")) ==
          "test_create_val_new";
  flag *= redis.read(std::make_pair("test_create_key", "dataField")) ==
          "someData_new";
  EXPECT_EQ(flag, true);
  redis.connection_->flushdb();
}

// RedisInterface::del.
TEST(CRUD, del) {
  checkRedisConnection(redis.connection_);
  redis.connection_->flushdb();
  bool flag = true;
  CreateRedis varFirst("test_create_key", "nfType", "test_create_val");
  flag *= redis.create(varFirst);
  CreateRedis varSecond("test_create_key", "dataField", "someData");
  flag *= redis.create(varSecond);
  redis.del(std::make_pair("test_create_key", "nfType"));
  redis.del(std::make_pair("test_create_key", "dataField"));
  flag *=
      redis.connection_->hget("test_create_key", "nfType") == "test_create_val";
  flag *= redis.connection_->hget("test_create_key", "dataField") == "someData";
  EXPECT_EQ(flag, false);
  redis.connection_->flushdb();
}

//===----------------------------------------------------------------------===//
// general methods.
//===----------------------------------------------------------------------===//

// RedisInterface::find.
TEST(FUNCTION, find) {
  checkRedisConnection(redis.connection_);
  redis.connection_->flushdb();

  const size_t n = 10;  // parameter for tune DB size.
  std::string path = path_dir + "../data/data_base.json";
  const char* cstr = path.c_str();
  redis.createUniDB(cstr, n);

  bool flag = true;
  for (const auto& type : nfTypes) {  // string_view???
    keys_t match_keys = redis.find(type);
    for (auto&& key : match_keys) {
      flag *= redis.connection_->hget(*key, "nfType") == type;
    }
  }

  EXPECT_EQ(flag, true);
  redis.connection_->flushdb();
}

// RedisInterface::createUniDB.
TEST(FUNCTION, createUniDB) {
  checkRedisConnection(redis.connection_);
  redis.connection_->flushdb();

  const size_t n = 10;  // parameter for tune DB size.
  std::string path = path_dir + "../data/data_base.json";
  const char* cstr = path.c_str();

  redis.createUniDB(cstr, n);

  bool flag = true;
  std::mt19937 rng;
  const uint8_t seed = 42;
  rng.seed(seed);
  std::uniform_int_distribution<int> dist(0, 20);
  for (size_t i = 0; i < n; ++i) {
    const uint8_t randomNumber = dist(rng);
    flag *= redis.connection_->hget(std::to_string(i), "nfType") ==
            nfTypes[randomNumber];
  }
  EXPECT_EQ(flag, true);
  redis.connection_->flushdb();
}
