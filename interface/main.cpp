#include "MongoDB/MongoDBInterface.h"
#include "RedisDB/RedisDBInterface.h"
#include <iostream>

int main(){
    MongoDBInterface mongo("mongodb://localhost:27017");
    mongo.set_db("test_db_nf");
    mongo.set_collection("test_collection");
    mongo.createUniDB("/home/anx_tl/cplusplus/project_nrf/tests/data/data_base.json", 1);
    auto cursor = mongo.find({});
    if (cursor.begin() == cursor.end()){
        std::cout << "fine" << std::endl;
    }
    for(auto i: cursor){
        std::string name = i["nfInstanceId"].get_utf8().value.to_string();
        std::cout << name << std::endl;
    }
    return 0;
}