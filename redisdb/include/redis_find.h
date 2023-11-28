#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <random>

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

    // create method.
    bool create(const StringView &key, const StringView &val);

    // create method for hash.
    bool hcreate(const StringView &key, 
                 const StringView &field, 
                 const StringView &dataField);

    // get method.
    OptionalString read(const StringView &key);

    // get method for hash.
    OptionalString hread(const StringView &key, const StringView &field);

    // update method.
    bool update(const StringView &key, const StringView &val);

    // update method.
    bool hupdate(const StringView &key, 
                 const StringView &field, 
                 const StringView &dataField);

    // del method.
    long long del(const StringView &key);

    // del method for hash.
    long long hdel(const StringView &key, const StringView &field);                   

    //===------------------------------------------------------------------===//
    // general methods.
    //===------------------------------------------------------------------===//

    // return nfprofile by nftype field.
    std::vector<OptionalString> find(const StringView &nfTypeSearch);

    // creating DB with uniform dist nfprofiles.
    void createUniDB(const char* config_path, const int n);
};
