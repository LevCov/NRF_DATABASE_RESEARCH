#include "RedisDBInterface.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "../UUID/UUIDv4.h"


using json = nlohmann::json;

bool RedisDBInterface::create(const std::pair<StrView, StrView>& key_val) {
    return conn->set(key_val.first, key_val.second);
}

OptStr RedisDBInterface::read(const StrView& key) {
    return conn->get(key);
}

bool RedisDBInterface::update(const StrView& key, const StrView& val) { 
    if (!(*this).read(key)) {
        (*this).create(std::pair<StrView, StrView>(key, val));
        return true;
    }
    return false;
    
}

void RedisDBInterface::del(const StrView& key) {
    conn->del(key);
}

bool RedisDBInterface::hcreate(const StrView &key, 
                             const StrView &field, 
                             const StrView &dataField) {
    conn->hset(key, field, dataField);
    return true;
}

OptStr RedisDBInterface::hread(const StrView &key, 
                                     const StrView &field) {                
    return conn->hget(key, field);
}

bool RedisDBInterface::hupdate(const StrView &key, 
                             const StrView &field, 
                             const StrView &dataField) {
    if (!conn->hget(key, field)) {
        conn->hset(key, field, dataField);
        return true;
    }
    return false;
}

long long RedisDBInterface::hdel(const StrView &key, const StrView &field) {                     
    return conn->hdel(key, field);
}
std::vector<OptStr> RedisDBInterface::find(const StrView &nfTypeSearch) {
    std::vector<OptStr> match_keys;
    std::vector<OptStr> keys;

    conn->keys("*", std::back_inserter(keys));
    for (const auto& key : keys) {        
        if (conn->hget(*key, "nfType") == nfTypeSearch)
            match_keys.push_back(key);
    }

    return match_keys;
}

// creating DB with uniform dist nfprofiles. 14064 byte or ~ 0,014 Mb per key.
void RedisDBInterface::createUniDB(const char* config_path, int n) {
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
                std::string nfInstanceId = UUID::New().String();
                const int randomNumber = dist(rng);
                profile["nfInstanceId"] = nfInstanceId;
                profile["nfType"] = nfTypes[randomNumber];
                conn->hset(nfInstanceId, "nfType", nfTypes[randomNumber]);
                conn->hset(nfInstanceId, "data", profile.dump());
            }
        }

        cf.close();   
    }
}
