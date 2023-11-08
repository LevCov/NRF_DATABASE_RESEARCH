#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>
#include <nlohmann/json.hpp>
#include <hiredis/hiredis.h>

class RedisInterface : public sw::redis::Redis {
public:
    using Redis::Redis;

    /// inditificator. 
    using Redis = sw::redis::Redis;
    using StringView = sw::redis::StringView;
    using OptionalString = sw::redis::OptionalString;
    using json = nlohmann::json;
    using ordered_json = nlohmann::ordered_json;

    //===------------------------------------------------------------------===//
    // CRUD redefined from the redis++ library.
    //===------------------------------------------------------------------===//

    // set method.
    bool create(const StringView &key, const StringView &val);

    // get method.
    OptionalString read(const StringView &key);

    // del method.
    long long del(const StringView &key);

    std::vector<OptionalString> find(const std::string nfTypeSearch);

    void loadingDB(const char* config_path);

    // benchmark test prototype.
    std::vector<std::vector<json>> benchmark(const char* nfinstance_path,
                                             const char* config_path);


    // shows whether config file is included with keys and values in nfprofile.
    std::vector<json> findJ2J(json nfinstance, json config_file);

    // shows whether json config file is included with keys and values 
    // in json nfprofile.
    std::vector<json> findP2P(const char* nfinstance_path, 
                              const char* config_path);

    // the same as the previous one, only we pass it by path.
    void find_code(json& config_file, 
                   json& profile, 
                   std::vector<json> &match_nfprofiles);


};
