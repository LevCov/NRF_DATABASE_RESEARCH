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

    /// @brief Set hash field to value.
    /// @param key Key where the hash is stored.
    /// @param field Field.
    /// @param val Value.
    void create(const StringView &key, 
                const StringView &field, 
                const StringView &dataField);

    /// @brief Get the value of the given field.
    /// @param key Key where the hash is stored.
    /// @param field Field.
    /// @return Value of the given field.
    OptionalString hread(const StringView &key, 
                         const StringView &field);

    /// @brief Set hash field to value, if not exist.
    /// @param key Key where the hash is stored.
    /// @param field Field.
    /// @param val Value.
    /// @note If field does not exist, `update` does nothing.
    void update(const StringView &key, 
                const StringView &field, 
                const StringView &dataField);

    /// @brief Remove the given field from hash.
    /// @param key Key where the hash is stored.
    /// @param field Field to be removed.
    void del(const StringView &key, const StringView &field);

    //===------------------------------------------------------------------===//
    // general methods.
    //===------------------------------------------------------------------===//

    /// @brief Find nfprofile by nftype field.
    /// @param key Key where the hash is stored.
    /// @param field Field to be removed.
    /// @return std::vector<OptionalString> of found keys.
    std::vector<OptionalString> find(const StringView &nfTypeSearch);
    
    /// @brief Сreating DB with uniform dist nfprofiles.
    /// @param config_path Path to file with template.
    /// @param field Size of database.
    void createUniDB(const char* config_path, const int n);
};
