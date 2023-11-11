//===----------------------------------------------------------------------===//

#include <gtest/gtest.h>
#include "../../include/redis_find.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>
#include <nlohmann/json.hpp>
#include <hiredis/hiredis.h>

using Redis = sw::redis::Redis;
using OptionalString = sw::redis::OptionalString;
using json = nlohmann::json;

const std::string path_dir = std::filesystem::current_path().remove_filename();
auto redis = std::make_unique<RedisInterface>("tcp://127.0.0.1:6379");

// RedisInterface::find.
TEST(FUNCTION, find) {
    redis->flushdb();

    std::string path = path_dir + "test_code/data/data_base.json";
    const char *cstr = path.c_str();
    redis->loadingDB(cstr);

    std::vector<OptionalString> match_keys = redis->find("NRF_TEST");

    for (const auto& key : match_keys) {
        EXPECT_EQ("string_TEST" == *key, true);
    }
    redis->flushdb();
}

// RedisInterface::loadingDB.
TEST(FUNCTION, loadingDB) {
    std::string path = path_dir + "test_code/data/data_base.json";
    const char *cstr = path.c_str();
    redis->loadingDB(cstr);
    EXPECT_EQ(*(redis->hget("string_TEST", "nfType")) == "NRF_TEST", true);
}

// RedisInterface::createDB.
TEST(FUNCTION, createDB) {
    redis->flushdb();
    bool flag = true;

    std::string path = path_dir + "test_code/data/data_base.json";
    const char *cstr = path.c_str();
    redis->createDB(cstr, 3, "TEST_NRF");

    for (int i = 0; i < 3; ++i) {
        flag *= redis->hget(std::to_string(i), "nfType") == "TEST_NRF";
    }
    EXPECT_EQ(flag, true);
    redis->flushdb();
}

// RedisInterface::create.
TEST(FUNCTION, create) {
    redis->flushdb();
    bool flag = true;
    flag *= redis->create("test_create_key", "test_create_val");
    flag *= redis->get("test_create_key") == "test_create_val";   
    EXPECT_EQ(flag, true);
    redis->flushdb();
}

// RedisInterface::read.
TEST(FUNCTION, read) {
    redis->flushdb();
    bool flag = true;
    flag *= redis->create("test_create_key", "test_create_val");
    flag *= redis->read("test_create_key") == "test_create_val";   
    EXPECT_EQ(flag, true);
    redis->flushdb();
}

// RedisInterface::del.
TEST(FUNCTION, del) {
    redis->flushdb();
    bool flag = true;
    flag *= redis->create("test_create_key", "test_create_val");
    redis->del("test_create_key");
    flag *= redis->get("test_create_key") == "test_create_val";   
    EXPECT_EQ(flag, false);
    redis->flushdb();
}
