//===----------------------------------------------------------------------===//

#include <gtest/gtest.h>
#include "../../include/redis_find.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>
#include <nlohmann/json.hpp>
#include <hiredis/hiredis.h>

using Redis = sw::redis::Redis;
using OptionalString = sw::redis::OptionalString;
using json = nlohmann::json;

const std::string path_dir = "/Users/georgryabov/Desktop/wtf/NRF_DATABASE_RESEARCH/redisdb/test/";

auto redis = std::make_unique<RedisInterface>("tcp://127.0.0.1:6379");

// loadingDB.
TEST(FUNCTION, loadingDB) {
    std::string path = path_dir + "test_code/data/data_base.json";
    const char *cstr = path.c_str();
    redis->loadingDB(cstr);
    EXPECT_EQ(*(redis->hget("string_TEST", "nfType")) == "NRF_TEST", true);
}

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

// find_code, expected true. test 1/3.
TEST(FUNCTION, find_code_true) {
    std::ifstream cf(path_dir + "test_code/data/config_file_test.json");
    std::ifstream pr(path_dir + "test_code/data/1profile.json");
    
    json config;
    json profile;

    std::vector<json> match_nfprofile;

    if (cf.is_open() && pr.is_open()) {
        config = json::parse(cf);
        profile = json::parse(pr);
        redis->find_code(config, profile, match_nfprofile);
        EXPECT_EQ((match_nfprofile[0].dump() == profile.dump()), true);
    } else {
        std::cout << "can not find the files\n";
        EXPECT_EQ(false, true); 
    }
}

// find_code, expected false (wrong key). test 2/3
TEST(FUNCTION, find_code_false_key) {
    std::ifstream cf(path_dir + "test_code/data/config_file_test.json");
    std::ifstream pr(path_dir + "test_code/data/1profile_wrong_key.json");
    
    json config;
    json profile;

    std::vector<json> match_nfprofile;

    if (cf.is_open() && pr.is_open()) {
        config = json::parse(cf);
        profile = json::parse(pr);
        redis->find_code(config, profile, match_nfprofile);
        EXPECT_EQ(match_nfprofile.empty(), true);
    } else {
        std::cout << "can not find the files\n";
        EXPECT_EQ(false, true); 
    }
}

// find_code, expected false (wrong value). test 3/3
TEST(FUNCTION, find_code_false_value) {
    std::ifstream cf(path_dir + "test_code/data/config_file_test.json");
    std::ifstream pr(path_dir + "test_code/data/1profile_wrong_value.json");
    
    json config;
    json profile;

    std::vector<json> match_nfprofile;

    if (cf.is_open() && pr.is_open()) {
        config = json::parse(cf);
        profile = json::parse(pr);
        redis->find_code(config, profile, match_nfprofile);
        EXPECT_EQ(match_nfprofile.empty(), true);
    } else {
        std::cout << "can not find the files\n";
        EXPECT_EQ(false, true); 
    }
}

// findJ2J, expected suitable nfprofiles. test 1/2
TEST(FUNCTION, findJ2J) {
    std::ifstream cf(path_dir + "test_code/data/config_file_test.json");
    std::ifstream inc(path_dir + "test_code/data/nfinstance.json");
    
    json config;
    json nfinstace;

    std::vector<json> match_nfprofiles;

    if (cf.is_open() && inc.is_open()) {
        config = json::parse(cf);
        nfinstace = json::parse(inc);
        match_nfprofiles = redis->findJ2J(nfinstace, config);
        EXPECT_EQ(match_nfprofiles.size() == 2, true);       // <-
    } else {
        std::cout << "can not find the files\n";
        EXPECT_EQ(false, true); 
    }
}

// findJ2J, expected suitable nfprofiles. test 2/2
TEST(FUNCTION, findJ2J_1) {
    std::ifstream cf(path_dir + "test_code/data/config_file_test.json");
    std::ifstream inc(path_dir + "test_code/data/2nfinstance.json");
    
    json config;
    json nfinstace;

    std::vector<json> match_nfprofiles;

    if (cf.is_open() && inc.is_open()) {
        config = json::parse(cf);
        nfinstace = json::parse(inc);
        match_nfprofiles = redis->findJ2J(nfinstace, config);
        EXPECT_EQ(match_nfprofiles.size() == 1, true);          // <-
    } else {
        std::cout << "can not find the files\n";
        EXPECT_EQ(false, true); 
    }
}
