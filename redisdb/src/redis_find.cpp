//===----------------------------------------------------------------------===//

#include <iostream>
#include <fstream>
#include <vector>

#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>
#include <nlohmann/json.hpp>
#include <hiredis/hiredis.h>

#include "../include/redis_find.h"

/// inditificator. 
using json = nlohmann::json;
using Redis = sw::redis::Redis;
using StringView = sw::redis::StringView;
using OptionalString = sw::redis::OptionalString;

/**
 ** \brief Represents a interface and search interactions in the redis database.
 */

//===----------------------------------------------------------------------===//
// CRUD redefined from the redis++ library.
//===----------------------------------------------------------------------===//

// set method, return bool.
bool RedisInterface::create(const StringView &key, const StringView &val) {
    return set(key, val);
}

// get method, return OptionalString.
OptionalString RedisInterface::read(const StringView &key) {                
    return get(key);
}        

// del method, return long long.
long long RedisInterface::del(const StringView &key) {                     
    return Redis::del(key);
}

// return the keys by search pattern.
std::vector<OptionalString> RedisInterface::find(
                                            const std::string nfTypeSearch) {
    std::vector<OptionalString> match_keys;
    std::vector<OptionalString> keys;

    (*this).keys("*", std::back_inserter(keys));
    for (const auto& key : keys) {        
        if ((*this).hget(*key, "nfType") == nfTypeSearch)
            match_keys.push_back(key);
    }

    return match_keys;
}

// loading data bate from the json by the json path.
void RedisInterface::loadingDB(const char* config_path) {
    std::ifstream cf(config_path); 

    if (cf.is_open()) {
        json instances = json::parse(cf);  

        for (auto& profile : instances) {      
            std::string nfInstanceId = profile["nfInstanceId"];
            std::string nfType = profile["nfType"];
            (*this).hset(nfInstanceId, "nfType", nfType);
            (*this).hset(nfInstanceId, "data", profile.dump(4));
        }

        cf.close();   
    }
}

// creating nfprofiles. 14064 byte or ~ 0,014 Mb per key.
void RedisInterface::createDB(const char* config_path, 
                              int n, 
                              std::string nfType) {
    std::ifstream cf(config_path); 

    if (cf.is_open()) {
        json instances = json::parse(cf);  

        for (auto& profile : instances) {  // for 1 of 1. 
            for (int i = 0; i < n; ++i) {
                const std::string nfInstanceId = std::to_string(i);
                profile["nfInstanceId"] = nfInstanceId;
                profile["nfType"] = nfType;
                (*this).hset(nfInstanceId, "nfType", nfType);
                (*this).hset(nfInstanceId, "data", profile.dump());
            }
        }

        cf.close();   
    }
}
