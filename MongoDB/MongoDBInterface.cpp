#include <MongoDBInterface.h>

#include <nlohmann/json.hpp>
#include <fstream>
#include <random>
#include "../UUID/UUIDv4.h"

using json = nlohmann::json;

void MongoDBInterface::set_db(const char* database){
    db = client.connection()->database(database);
}

void MongoDBInterface::set_collection(const char* collection){
    collect = db[collection];
}

void MongoDBInterface::drop(){
    db.drop();
}

bool MongoDBInterface::create(const bsoncxx::document::view_or_value& document) {
    collect.insert_one(document.view());
    return true;
}

mongocxx::cursor MongoDBInterface::read(const bsoncxx::document::view_or_value& read){
    return collect.find(read);
}

bool MongoDBInterface::update(const bsoncxx::document::view_or_value& filter, const bsoncxx::document::view_or_value& update) {        
    auto val = collect.update_one(filter, update);
    if(val.value().modified_count()){
        return true;
    }
    return false;
}

void MongoDBInterface::del(const bsoncxx::document::view_or_value& filter) {
    collect.delete_one(filter);
}

mongocxx::cursor MongoDBInterface::find(const bsoncxx::document::view_or_value& filter){
    mongocxx::cursor result = collect.find(filter);/*
    if(result.begin() == result.end()){
        std::cout << "There are no matches" << std::endl;
    }*/
    return result;
}

bsoncxx::document::value MongoDBInterface::from_JSON(const char *path)
{
    std::ifstream cf(path); 
    if(cf.is_open()){
        json file = json::parse(cf); 
        return bsoncxx::from_json(file.dump());
    }
    return bsoncxx::builder::basic::make_document();
}

void MongoDBInterface::createUniDB(const char *config_path, int n)
{
    std::ifstream cf(config_path); 

    if (cf.is_open()) {
        std::vector<std::string> nfTypes {"NRF", "UDM", "AMF", "SMF", "AUSF", 
                                          "NEF", "PCF", "SMSF", "NSSF", "UDR", 
                                          "LMF", "GMLC", "5G_EIR", "SEPP", "UPF",
                                          "N3IWF", "AF", "UDSF", "BSF", "CHF", 
                                          "NWDAF"}; // 20 nftypes.

        std::mt19937 rng;
        const unsigned int seed = 42;
        rng.seed(seed);
        std::uniform_int_distribution<int> dist(0, 20);
        json instances = json::parse(cf);  
        
        for (auto& profile : instances) {  // for 1 of 1. 
            for (int i = 0; i < n; i++) {
                //const std::string nfInstanceId = std::to_string(i);
                const int randomNumber = dist(rng);
                profile["nfInstanceId"] = UUID::New().String();
                profile["nfType"] = nfTypes[randomNumber];
                create(bsoncxx::from_json(profile.dump()));
            }
        }

        cf.close();   
    }
}
