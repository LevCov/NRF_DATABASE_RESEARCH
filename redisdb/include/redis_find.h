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

    // return nfprofile by nftype field.
    std::vector<OptionalString> find(const std::string nfTypeSearch);

    // loading nfprofiles in Redis data base.
    void loadingDB(const char* config_path);

    // creating database with sizedb nfprofiles.
    void createDB(const char* config_path, int sizedb, std::string nfType);
};
