//===----------------------------------------------------------------------===//

#include <gtest/gtest.h>
#include "../../RedisDB/RedisDBInterface.h"

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

using OptionalString = sw::redis::OptionalString;
using StrView = sw::redis::StringView;
using json = nlohmann::json;

const std::string path_dir = std::filesystem::current_path().remove_filename();
RedisDBInterface redis("tcp://127.0.0.1:6379");
//===----------------------------------------------------------------------===//
// CRUD redefined from the redis++ library.
//===----------------------------------------------------------------------===//

// RedisInterface::create.  
TEST(CRUD, create) {
    redis.conn->flushdb();
    bool flag = true;
    flag *= redis.create(std::pair<StrView, StrView>("test_create_key", "test_create_val"));
    flag *= redis.conn->get("test_create_key") == "test_create_val";   
    EXPECT_EQ(flag, true);
    redis.conn->flushdb();
}

// RedisInterface::hcreate.  
TEST(CRUD, hcreate) {
    redis.conn->flushdb();
    bool flag = true;
    flag *= redis.hcreate("test_create_key", "nfType", "test_create_val");
    flag *= redis.conn->hget("test_create_key", "nfType") == "test_create_val";   
    EXPECT_EQ(flag, true);
    redis.conn->flushdb();
}

// RedisInterface::read.
TEST(CRUD, read) {
    redis.conn->flushdb();
    bool flag = true;
    flag *= redis.create(std::pair<StrView, StrView>("test_create_key", "test_create_val"));
    flag *= redis.read("test_create_key") == "test_create_val";   
    EXPECT_EQ(flag, true);
    redis.conn->flushdb();
}

// RedisInterface::hread.
TEST(CRUD, hread) {
    redis.conn->flushdb();
    bool flag = true;
    flag *= redis.hcreate("test_create_key", "nfType", "test_create_val");
    flag *= redis.hcreate("test_create_key", "dataField", "someData");
    flag *= redis.hread("test_create_key", "nfType") == "test_create_val";   
    flag *= redis.hread("test_create_key", "dataField") == "someData";   
    EXPECT_EQ(flag, true);
    redis.conn->flushdb();
}

// RedisInterface::update with exist key.
TEST(CRUD, update_exist) {
    redis.conn->flushdb();
    bool flag = true;
    redis.create(std::pair<StrView, StrView>("test_create_key", "test_create_val"));
    flag *= redis.update("test_create_key", "test_create_val");   
    EXPECT_EQ(flag, false);
    redis.conn->flushdb();
}

// RedisInterface::update with no exist key.
TEST(CRUD, update_no_exist) {
    redis.conn->flushdb();
    bool flag = true;
    flag *= redis.update("test_create_key", "test_create_val");   
    EXPECT_EQ(flag, true);
    redis.conn->flushdb();
}

// RedisInterface::hupdate with exist key.
TEST(CRUD, hupdate_exist) {
    redis.conn->flushdb();
    bool flag = true;
    flag *= redis.hcreate("test_create_key", "nfType", "test_create_val");
    flag *= redis.hcreate("test_create_key", "dataField", "someData");
    flag *= redis.hupdate("test_create_key", "nfType", "test_create_val_new");  
    flag *= redis.hupdate("test_create_key", "dataField", "someData_new");  
    EXPECT_EQ(flag, false);
    redis.conn->flushdb();
}

// RedisInterface::hupdate with no exist key.
TEST(CRUD, hupdate_no_exist) {
    redis.conn->flushdb();
    bool flag = true;
    flag *= redis.hupdate("test_create_key", "nfType", "test_create_val_new");
    flag *= redis.hupdate("test_create_key", "dataField", "someData_new");
    flag *= redis.hread("test_create_key", "nfType") == "test_create_val_new";   
    flag *= redis.hread("test_create_key", "dataField") == "someData_new"; 
    EXPECT_EQ(flag, true);
    redis.conn->flushdb();
}

// RedisInterface::del.
TEST(CRUD, del) {
    redis.conn->flushdb();
    bool flag = true;
    flag *= redis.create(std::pair<StrView, StrView>("test_create_key", "test_create_val"));
    redis.del("test_create_key");
    flag *= redis.conn->get("test_create_key") == "test_create_val";   
    EXPECT_EQ(flag, false);
    redis.conn->flushdb();
}

// RedisInterface::hdel.
TEST(CRUD, hdel) {
    redis.conn->flushdb();
    bool flag = true;
    flag *= redis.hcreate("test_create_key", "nfType", "test_create_val");
    flag *= redis.hcreate("test_create_key", "dataField", "someData");
    redis.hdel("test_create_key", "nfType");
    redis.hdel("test_create_key", "dataField");
    flag *= redis.conn->hget("test_create_key", "nfType") == "test_create_val";   
    flag *= redis.conn->hget("test_create_key", "dataField") == "someData";   
    EXPECT_EQ(flag, false);
    redis.conn->flushdb();
}

//===----------------------------------------------------------------------===//
// general methods.
//===----------------------------------------------------------------------===//

// RedisInterface::find.
TEST(FUNCTION, find) {
    redis.conn->flushdb();
    std::vector<std::string> nfTypes {"NRF", "UDM", "AMF", "SMF", "AUSF", 
                                      "NEF", "PCF", "SMSF", "NSSF", "UDR", 
                                      "LMF", "GMLC", "5G_EIR", "SEPP", "UPF",
                                      "N3IWF", "AF", "UDSF", "BSF", "CHF", 
                                      "NWDAF"};
    bool flag = true;
    const int n = 10;   // parameter for tune DB size.
    std::string path = path_dir + "../data/data_base.json";
    const char *cstr = path.c_str();

    redis.createUniDB(cstr, n);

    for (auto& type : nfTypes) {            //string_view???
        std::vector<OptionalString> match_keys = redis.find(type);
        for (auto& key : match_keys) {
            flag *= redis.conn->hget(*key, "nfType") == type;
        }
    }

    EXPECT_EQ(flag, true);
    redis.conn->flushdb();
}

// RedisInterface::createUniDB.
TEST(FUNCTION, createUniDB) {
    redis.conn->flushdb();
    std::vector<std::string> nfTypes {"NRF", "UDM", "AMF", "SMF", "AUSF", 
                                      "NEF", "PCF", "SMSF", "NSSF", "UDR", 
                                      "LMF", "GMLC", "5G_EIR", "SEPP", "UPF",
                                      "N3IWF", "AF", "UDSF", "BSF", "CHF", 
                                      "NWDAF"};
    bool flag = true;
    const int n = 100;     // parameter for tune DB size. 

    std::string path = path_dir + "../data/data_base.json";
    const char *cstr = path.c_str();
    
    redis.createUniDB(cstr, n);

    std::mt19937 rng;
    unsigned int seed = 42;
    rng.seed(seed);
    std::uniform_int_distribution<int> dist(0, 20);

    for (int i = 0; i < n; ++i) {       
        const int randomNumber = dist(rng);
        flag *= *(redis.conn->hget(std::to_string(i), "nfType")) == nfTypes[randomNumber];
    }
    EXPECT_EQ(flag, true);
    redis.conn->flushdb();
}
