//===----------------------------------------------------------------------===//

#include <iostream>
#include <fstream>
#include <vector>
#include <random>

#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>
#include <nlohmann/json.hpp>
#include <hiredis/hiredis.h>
// #include "uuid_v4/endianness.h"
// #include "uuid_v4/uuid_v4.h"

#include "../include/redis_find.h"
#include "../include/nfTypes.h"

/// inditificator. 
using json = nlohmann::json;
using Redis = sw::redis::Redis;
using StringView = sw::redis::StringView;
using OptionalString = sw::redis::OptionalString;

/**
 ** \brief Represents a interface and search interactions in the redis database.
 */

void RedisInterface::create(const StringView &key, 
                            const StringView &field, 
                            const StringView &dataField) {
    (*this).hset(key, field, dataField);
}

OptionalString RedisInterface::hread(const StringView &key, 
                                     const StringView &field) {                
    return (*this).hget(key, field);
}        

void RedisInterface::update(const StringView &key, 
                            const StringView &field, 
                            const StringView &dataField) {
    if (!(*this).hget(key, field)) {
        (*this).hset(key, field, dataField);
    }
}

void RedisInterface::del(const StringView &key, const StringView &field) {                     
    Redis::hdel(key, field);
}

//===----------------------------------------------------------------------===//
// general methods.
//===----------------------------------------------------------------------===//

std::vector<OptionalString> RedisInterface::find(
                                            const StringView &nfTypeSearch) {
    std::vector<OptionalString> match_keys;
    std::vector<OptionalString> keys;

    (*this).keys("*", std::back_inserter(keys));
    for (const auto& key : keys) {        
        if ((*this).hget(*key, "nfType") == nfTypeSearch)
            match_keys.push_back(key);
    }

    return match_keys;
}

void RedisInterface::createUniDB(const char* config_path, const int n) {
    std::ifstream cf(config_path); 
    if (cf.is_open()) {
        std::mt19937 rng;
        const unsigned int seed = 42;
        rng.seed(seed);
        std::uniform_int_distribution<int> dist(0, 21);
        
        json instances = json::parse(cf);  
        for (auto& profile : instances) {  // for 1 of 1. 
        for (int i = 0; i < n; ++i) {
            const std::string nfInstanceId = std::to_string(i);
            const int randomNumber = dist(rng);
            profile["nfInstanceId"] = nfInstanceId;
            profile["nfType"] = nfTypes[randomNumber];
            (*this).create(nfInstanceId, "nfType", nfTypes[randomNumber]);
            (*this).create(nfInstanceId, "data", profile.dump());
        }
        }
        cf.close();   
    }
}
