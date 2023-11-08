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

// set method.
bool RedisInterface::create(const StringView &key, const StringView &val) {
    return set(key, val);
}

// get method.
OptionalString RedisInterface::read(const StringView &key) {                
    return get(key);
}        

// del method.
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
    json instances = json::parse(cf);  

    for (auto& profile : instances) {      
        std::string nfInstanceId = profile["nfInstanceId"];
        std::string nfType = profile["nfType"];
        (*this).hset(nfInstanceId, "nfType", nfType);
        (*this).hset(nfInstanceId, "data", profile.dump(4));
    }
}

/// last code.

// shows whether config file is included with keys and values in nfprofile.
void RedisInterface::find_code(json& config_file, 
                json& profile, 
                std::vector<json> &match_nfprofiles) {
    bool flag = true;
    for (auto& el : config_file.items()) {    
        if (flag *= profile.contains(el.key()))
            flag *= profile[el.key()] == config_file[el.key()];
        if (!flag)
            break;
    }
    if (flag) {
        match_nfprofiles.push_back(profile);
    }
}

// shows whether json config file is included with keys and values 
// in json nfprofile.
std::vector<json> RedisInterface::findJ2J(json nfinstance, json config_file) {
    std::vector<json> match_nfprofiles;    

    // !!manually var. 90 is the keys in the nfprofile.
    if (nfinstance.begin().value().size() == 90) {       // for nfinstance.
        for (auto& profile : nfinstance) {
            find_code(config_file, profile, match_nfprofiles);
        }
    } else {                                             // for nfprofile.
        find_code(config_file, nfinstance, match_nfprofiles);
    }
    
    return match_nfprofiles;
}  


// the same as the previous one, only we pass it by path.
std::vector<json> RedisInterface::findP2P(const char* nfinstance_path,
                                          const char* config_path) {
    std::vector<json> match_nfprofiles;

    std::ifstream f(nfinstance_path);
    std::ifstream cf(config_path); 

    json nfinstance = json::parse(f);
    json config_file = json::parse(cf);    

    // !!manually var. 90 is the keys in the nfprofile.
    if (nfinstance.begin().value().size() == 90) {
        for (auto& profile : nfinstance) {
            find_code(config_file, profile, match_nfprofiles);
        }
    } else {
        find_code(config_file, nfinstance, match_nfprofiles);
    }

    return match_nfprofiles;
}  
