//===----------------------------------------------------------------------===//

#include <gtest/gtest.h>
#include "../../MongoDB/MongoDBInterface.h"

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <random>
#include <string>

using json = nlohmann::json;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

const std::string path_dir = std::filesystem::current_path().remove_filename();
MongoDBInterface mongo("mongodb://localhost:27017");
//===----------------------------------------------------------------------===//
// CRUD redefined from the MONGOCXX library.
//===----------------------------------------------------------------------===//

// RedisInterface::create.  
TEST(CRUD, create) {
    mongo.set_db("NRF_DB");
    mongo.set_collection("NFs");
    bool flag = true;
    auto doc_value = make_document(
        kvp("name", "MongoDB"),
        kvp("type", "database"),
        kvp("count", 1));

    flag *= mongo.create(doc_value.view());
    auto doc = mongo.collect.find_one(make_document(kvp("name", "MongoDB")));

    if(doc){
        bsoncxx::document::value docVal = doc.value();
        std::string name = docVal["name"].get_utf8().value.to_string();
        std::cout << name << std::endl;
        name = docVal["type"].get_utf8().value.to_string();
        std::cout << name << std::endl;
        flag *= name == "database";
        int x = docVal["count"].get_int32().value;
        std::cout << x << std::endl;
        flag *= x == 1;
    }
    else{
        flag *= 0;
    }
    EXPECT_EQ(flag, true);
    mongo.drop();
}


// RedisInterface::read.
TEST(CRUD, read) {
    mongo.set_db("NRF_DB");
    mongo.set_collection("NFs");
    bool flag = true;
    auto doc_value = make_document(
        kvp("name", "MongoDB"),
        kvp("type", "database"),
        kvp("count", 1));

    mongo.create(doc_value.view());
    auto cursor = mongo.read(make_document(kvp("name", "MongoDB")));
    if (cursor.begin() == cursor.end()){
        flag *= 0;
    }
    for(auto docVal: cursor){
        std::string name = docVal["name"].get_utf8().value.to_string();
        std::cout << name << std::endl;
        name = docVal["type"].get_utf8().value.to_string();
        std::cout << name << std::endl;
        flag *= name == "database";
        int x = docVal["count"].get_int32().value;
        std::cout << x << std::endl;
        flag *= x == 1;
    }
    EXPECT_EQ(flag, true);
    mongo.drop();
}

// RedisInterface::update with exist key.
TEST(CRUD, update_exist) {
    mongo.set_db("NRF_DB");
    mongo.set_collection("NFs");
    bool flag = true;
    auto doc_value = make_document(
        kvp("name", "MongoDB"),
        kvp("type", "database"),
        kvp("count", 1));

    mongo.create(doc_value.view());
    flag *= mongo.update(make_document(
                            kvp("name", "MongoDB")
                        ), 
                        make_document(
                            kvp("$set", make_document(
                                kvp("name", "mongodb")
                                ))
                            ));   
    EXPECT_EQ(flag, true);
    mongo.drop();
}

// RedisInterface::update with no exist key.
TEST(CRUD, update_no_exist) {
    mongo.set_db("NRF_DB");
    mongo.set_collection("NFs");
    bool flag = true;
    auto doc_value = make_document(
        kvp("name", "MongoDB"),
        kvp("type", "database"),
        kvp("count", 1));

    mongo.create(doc_value.view());
    flag *= mongo.update(make_document(
                            kvp("----", 2)
                        ), 
                        make_document(
                            kvp("$set", make_document(
                                kvp("count", 3)
                                ))
                            ));   
    EXPECT_EQ(flag, false);
    mongo.drop();
}



// RedisInterface::del.
TEST(CRUD, del) {
    mongo.set_db("NRF_DB");
    mongo.set_collection("NFs");
    bool flag = true;
    auto doc_value = make_document(
        kvp("name", "MongoDB"),
        kvp("type", "database"),
        kvp("count", 1));

    flag *= mongo.create(doc_value.view());
    mongo.del(make_document(
                    kvp("name", "MongoDB")
                    ));
    auto cursor = mongo.read(make_document(kvp("name", "MongoDB")));
    if (cursor.begin() == cursor.end()){
        flag *= 1;
    }
    EXPECT_EQ(flag, true);
    mongo.drop();
}


//===----------------------------------------------------------------------===//
// general methods.
//===----------------------------------------------------------------------===//

TEST(FUNCTION, find) {
    mongo.set_db("NRF_DB");
    mongo.set_collection("NFs");
    std::vector<std::string> nfTypes {"NRF", "UDM", "AMF", "SMF", "AUSF", 
                                      "NEF", "PCF", "SMSF", "NSSF", "UDR", 
                                      "LMF", "GMLC", "5G_EIR", "SEPP", "UPF",
                                      "N3IWF", "AF", "UDSF", "BSF", "CHF", 
                                      "NWDAF"};
    bool flag = true;
    const int n = 10;   // parameter for tune DB size.
    std::string path = path_dir + "../data/data_base.json";
    const char *cstr = path.c_str();

    mongo.createUniDB(cstr, n);

    for (auto& type : nfTypes) {            
        mongocxx::cursor match = mongo.find(make_document(kvp("nfType", type)));
        for (auto doc : match) {
            std::string type_f = doc["nfType"].get_utf8().value.to_string();
            flag *= type_f == type;
        }
    }

    EXPECT_EQ(flag, true);
    mongo.drop();
}

TEST(FUNCTION, createUniDB) {
    mongo.set_db("NRF_DB");
    mongo.set_collection("NFs");
    std::vector<std::string> nfTypes {"NRF", "UDM", "AMF", "SMF", "AUSF", 
                                      "NEF", "PCF", "SMSF", "NSSF", "UDR", 
                                      "LMF", "GMLC", "5G_EIR", "SEPP", "UPF",
                                      "N3IWF", "AF", "UDSF", "BSF", "CHF", 
                                      "NWDAF"};
    bool flag = true;
    const int n = 100;     // parameter for tune DB size. 

    std::string path = path_dir + "../data/data_base.json";
    const char *cstr = path.c_str();
    
    mongo.createUniDB(cstr, n);

    std::mt19937 rng;
    unsigned int seed = 42;
    rng.seed(seed);
    std::uniform_int_distribution<int> dist(0, 20);

    for (int i = 0; i < n; ++i) {       
        const int randomNumber = dist(rng);
        auto cursor = mongo.read(make_document(kvp("nfInstanceId", std::to_string(randomNumber))));
        if (cursor.begin() == cursor.end()){
            flag *= 0;
            break;
        }
    }
    EXPECT_EQ(flag, true);
    mongo.drop();
}
