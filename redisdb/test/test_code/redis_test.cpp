//===----------------------------------------------------------------------===//

#include "../../include/redis_find.h"
#include "../../include/nfTypes.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include <gtest/gtest.h>
#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>
#include <nlohmann/json.hpp>
#include <hiredis/hiredis.h>

using Redis = sw::redis::Redis;
using OptionalString = sw::redis::OptionalString;
using json = nlohmann::json;

const std::string path_dir = std::filesystem::current_path().remove_filename();
auto redis = std::make_unique<RedisInterface>("tcp://127.0.0.1:6379");

//===----------------------------------------------------------------------===//
// CRUD redefined from the redis++ library.
//===----------------------------------------------------------------------===//

// RedisInterface::create.  
TEST(CRUD, create) {
    redis->flushdb();
    redis->create("test_create_key", "nfType", "test_create_val");
    bool flag = redis->hget("test_create_key", "nfType") == "test_create_val";   
    EXPECT_EQ(flag, true);
    redis->flushdb();
}

// // RedisInterface::read.
// TEST(CRUD, read) {
//     redis->flushdb();
//     bool flag = true;
//     flag *= redis->create("test_create_key", "test_create_val");
//     flag *= redis->read("test_create_key") == "test_create_val";   
//     EXPECT_EQ(flag, true);
//     redis->flushdb();
// }

// RedisInterface::read.
// TEST(CRUD, read) {
//     redis->flushdb();
//     bool flag = true;
//     flag *= redis->hset("test_create_key", "nfType", "test_create_val");
//     flag *= redis->hset("test_create_key", "dataField", "someData");
//     flag *= redis->read("test_create_key", "nfType") == "test_create_val";   
//     flag *= redis->read("test_create_key", "dataField") == "someData";   
//     EXPECT_EQ(flag, true);
//     redis->flushdb();
// }

// RedisInterface::update with exist key.
TEST(CRUD, update_exist) {
    redis->flushdb();
    bool flag = true;
    redis->hset("test_create_key", "nfType", "test_create_val");
    redis->hset("test_create_key", "dataField", "someData");
    redis->update("test_create_key", "nfType", "test_create_val_new");  // no changes because key exist.
    redis->update("test_create_key", "dataField", "someData_new");      // the same.
    flag *= *redis->hget("test_create_key", "nfType")    == "test_create_val";
    flag *= *redis->hget("test_create_key", "dataField") == "someData";
    EXPECT_EQ(flag, true);
    redis->flushdb();
}

// RedisInterface::update with no exist key.
TEST(CRUD, update_no_exist) {
    redis->flushdb();
    bool flag = true;
    redis->update("test_create_key", "nfType", "test_create_val_new");
    redis->update("test_create_key", "dataField", "someData_new");
    flag *= redis->hget("test_create_key", "nfType") == "test_create_val_new";   
    flag *= redis->hget("test_create_key", "dataField") == "someData_new"; 
    EXPECT_EQ(flag, true);
    redis->flushdb();
}

// RedisInterface::del.
TEST(CRUD, del) {
    redis->flushdb();
    bool flag = true;
    redis->hset("test_create_key", "nfType", "test_create_val");
    redis->hset("test_create_key", "dataField", "someData");
    redis->del("test_create_key", "nfType");
    redis->del("test_create_key", "dataField");
    flag *= redis->hget("test_create_key", "nfType") == "test_create_val";   
    flag *= redis->hget("test_create_key", "dataField") == "someData";   
    EXPECT_EQ(flag, false);
    redis->flushdb();
}

// //===----------------------------------------------------------------------===//
// // general methods.
// //===----------------------------------------------------------------------===//

// RedisInterface::find.
TEST(FUNCTION, find) {
    redis->flushdb();
    bool flag = true;
    const int n = 10;   // parameter for tune DB size.
    std::string path = path_dir + "test_code/data/data_base.json";
    const char *cstr = path.c_str();
    redis->createUniDB(cstr, n);

    for (auto& type : nfTypes) {            
        std::vector<OptionalString> match_keys = redis->find(type);
        for (auto& key : match_keys) {
            flag *= redis->hget(*key, "nfType") == type;
        }
    }

    EXPECT_EQ(flag, true);
    redis->flushdb();
}

// RedisInterface::createUniDB.
TEST(FUNCTION, createUniDB) {
    redis->flushdb();
    bool flag = true;
    const int n = 100;     // parameter for tune DB size. 
    std::string path = path_dir + "test_code/data/data_base.json";
    const char *cstr = path.c_str();
    redis->createUniDB(cstr, n);

    std::mt19937 rng;
    unsigned int seed = 42;
    rng.seed(seed);
    std::uniform_int_distribution<int> dist(0, 21);

    for (int i = 0; i < n; ++i) {       
        const int randomNumber = dist(rng);
        flag *= redis->hget(std::to_string(i), "nfType") == nfTypes[randomNumber];
    }

    EXPECT_EQ(flag, true);
    redis->flushdb();
}
