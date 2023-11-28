//===----------------------------------------------------------------------===//

#include <iostream>
#include <fstream>
#include <vector>
#include <random>

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

// set method for hash, return bool.
bool RedisInterface::hcreate(const StringView &key, 
                             const StringView &field, 
                             const StringView &dataField) {
    (*this).hset(key, field, dataField);
    return true;
}

// get method, return OptionalString.
OptionalString RedisInterface::read(const StringView &key) {                
    return get(key);
}        

// get method for hash, return OptionalString.
OptionalString RedisInterface::hread(const StringView &key, 
                                     const StringView &field) {                
    return (*this).hget(key, field);
}        

// update method, return long long.
bool RedisInterface::update(const StringView &key, const StringView &val) {
    if (!(*this).read(key)) {
        (*this).create(key, val);
        return true;
    }
    return false;
}

// update method for hash, return long long.
bool RedisInterface::hupdate(const StringView &key, 
                             const StringView &field, 
                             const StringView &dataField) {
    if (!(*this).hget(key, field)) {
        (*this).hset(key, field, dataField);
        return true;
    }
    return false;
}

// del method, return long long.
long long RedisInterface::del(const StringView &key) {                     
    return Redis::del(key);
}

// del method for hash, return long long.
long long RedisInterface::hdel(const StringView &key, const StringView &field) {                     
    return Redis::hdel(key, field);
}

//===----------------------------------------------------------------------===//
// general methods.
//===----------------------------------------------------------------------===//

// return the keys by search pattern.
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

// creating DB with uniform dist nfprofiles. 14064 byte or ~ 0,014 Mb per key.
void RedisInterface::createUniDB(const char* config_path, const int n) {
    std::ifstream cf(config_path); 

    if (cf.is_open()) {
        std::vector<std::string> nfTypes {"NRF", "UDM", "AMF", "SMF", "AUSF", 
                                          "NEF", "PCF", "SMSF", "NSSF", "UDR", 
                                          "LMF", "GMLC", "5G_EIR", "SEPP", "UPF"
                                          "N3IWF", "AF", "UDSF", "BSF", "CHF", 
                                          "NWDAF"}; // 20 nftypes.

        std::mt19937 rng;
        const unsigned int seed = 42;
        rng.seed(seed);
        std::uniform_int_distribution<int> dist(0, 20);
        json instances = json::parse(cf);  

        for (auto& profile : instances) {  // for 1 of 1. 
            for (int i = 0; i < n; ++i) {
                const std::string nfInstanceId = std::to_string(i);
                const int randomNumber = dist(rng);
                profile["nfInstanceId"] = nfInstanceId;
                profile["nfType"] = nfTypes[randomNumber];
                (*this).hset(nfInstanceId, "nfType", nfTypes[randomNumber]);
                (*this).hset(nfInstanceId, "data", profile.dump());
            }
        }

        cf.close();   
    }
}
