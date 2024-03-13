//===----------------------------------------------------------------------===//

#include <gtest/gtest.h>
#include "../../RocksDB/RocksDBInterface.h"

#include <fstream>
#include <filesystem>
#include <random>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string path_dir = std::filesystem::current_path().remove_filename();
RocksDBInterface rocks("/tmp/rocksdb");
//===----------------------------------------------------------------------===//
// CRUD redefined from the rocksdb library.
//===----------------------------------------------------------------------===//

// RocksInterface::create.  
TEST(CRUD, create) {
    rocks.flushdb();
    bool flag = true;
    flag *= rocks.create(Create{0, "key_test", "value_test"});

    std::string val;
    rocks.conn->Get(ReadOptions(), rocks.handles[0], "key_test", &val);

    flag *=  val == "value_test";   
    EXPECT_EQ(flag, true);
    rocks.flushdb();
}

// RocksInterface::read.
TEST(CRUD, read) {
    rocks.flushdb();
    bool flag = true;
    flag *= rocks.create(Create{0, "key_test", "value_test"});

    Read val = rocks.read(Del_Read{0, "key_test"}); 
    flag *= val.value == "value_test";
    EXPECT_EQ(flag, true);
    rocks.flushdb();
}

// RocksInterface::update with exist key.
TEST(CRUD, update_exist) {
    rocks.flushdb();
    bool flag = true;
    rocks.create(Create{0, "key_test", "value_test"});
    flag *= rocks.update(Update{0, "key_test", ""}, Update{0, "", "value_update"});   
    EXPECT_EQ(flag, false);
    rocks.flushdb();
}

// RocksInterface::update with no exist key.
TEST(CRUD, update_no_exist) {
    rocks.flushdb();
    bool flag = true;
    flag *= rocks.update(Update{0, "key_test", ""}, Update{0, "", "value_update"});  
    EXPECT_EQ(flag, true);
    rocks.flushdb();
}

// RocksInterface::del.
TEST(CRUD, del) {
    rocks.flushdb();
    bool flag = true;
    rocks.create(Create{0, "key_test", "value_test"});
    Read val = rocks.read(Del_Read{0, "key_test"}); 
    flag *= val.value == "value_test";
    rocks.del(Del_Read{0, "key_test"});
    std::string v;
    auto s = rocks.conn->Get(ReadOptions(), rocks.handles[0], "test_key", &v);
    flag *= !s.ok();
    EXPECT_EQ(flag, true);
    rocks.flushdb();
}

//===----------------------------------------------------------------------===//
// general methods.
//===----------------------------------------------------------------------===//

// RocksInterface::find.
TEST(FUNCTION, find) {
    rocks.flushdb();
    std::vector<std::string> nfTypes {"NRF", "UDM", "AMF", "SMF", "AUSF", 
                                      "NEF", "PCF", "SMSF", "NSSF", "UDR", 
                                      "LMF", "GMLC", "5G_EIR", "SEPP", "UPF",
                                      "N3IWF", "AF", "UDSF", "BSF", "CHF", 
                                      "NWDAF"};
    bool flag = true;
    const int n = 10;
    std::string path = path_dir + "../data/data_base.json";
    const char *cstr = path.c_str();

    rocks.createUniDB(cstr, n);

    for (auto& type : nfTypes) {
        auto match_keys = rocks.find(FindPar{rocks.NF[type]});
        for (auto& key : match_keys.values) {
            auto val = rocks.read(Del_Read{
                rocks.NF[type], key
            });
            json instances = json::parse(val.value);
            flag *= instances["nfType"] == type;
        }
    }

    EXPECT_EQ(flag, true);
    rocks.flushdb();
}

// RocksInterface::createUniDB.
TEST(FUNCTION, createUniDB) {
    rocks.flushdb();
    std::vector<std::string> nfTypes {"NRF", "UDM", "AMF", "SMF", "AUSF", 
                                      "NEF", "PCF", "SMSF", "NSSF", "UDR", 
                                      "LMF", "GMLC", "5G_EIR", "SEPP", "UPF",
                                      "N3IWF", "AF", "UDSF", "BSF", "CHF", 
                                      "NWDAF"};
    bool flag = true;
    const int n = 100;     // parameter for tune DB size. 

    std::string path = path_dir + "../data/data_base.json";
    const char *cstr = path.c_str();
    
    rocks.createUniDB(cstr, n);

    std::mt19937 rng;
    unsigned int seed = 42;
    rng.seed(seed);
    std::uniform_int_distribution<int> dist(0, 20);

    for (int i = 0; i < n; ++i) {       
        const int randomNumber = dist(rng);
        Read val = rocks.read(Del_Read{rocks.NF[nfTypes[randomNumber]], std::to_string(i)});
        json instances = json::parse(val.value);
        flag *= instances["nfType"] == nfTypes[randomNumber];
    }
    EXPECT_EQ(flag, true);
    rocks.flushdb();
}
