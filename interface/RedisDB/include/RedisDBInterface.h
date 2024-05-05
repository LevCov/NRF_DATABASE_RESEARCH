#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../../include/DBInterface.h"
#include "../../include/connection.h"
#include "redis_conn.h"

using OptionalString = sw::redis::OptionalString;
using StringView = sw::redis::StringView;

struct CreateRedis;

/// @brief Interface to the redis database.
class RedisDBInterface final
    : public DBInterface<CreateRedis, OptionalString,
                         std::pair<StringView, StringView>,
                         std::pair<StringView, StringView>, StringView,
                         std::vector<OptionalString>> {
 public:
  /// @brief Redis initialization.
  /// @param uri The server at the address 127.0.0.1 with port 6379.
  explicit RedisDBInterface(const char* uri)
      : redis_(uri), connection_{redis_.connection()} {}

  RedisDBInterface() = delete;
  RedisDBInterface(const RedisDBInterface& rhs) = delete;
  RedisDBInterface& operator=(const RedisDBInterface& rhs) = delete;
  RedisDBInterface(const RedisDBInterface&& rhs) = delete;
  RedisDBInterface&& operator=(const RedisDBInterface&& rhs) = delete;
  ~RedisDBInterface() = default;

  /// @brief Creating a hash object in the redis database.
  /// @param var The parameter is a structure with a key where the hash is
  /// stored, field and value.
  /// @return Always true.
  bool create(const CreateRedis& var) override;

  /// @brief Reading a hash object from the redis database.
  /// @param key Key where the hash is stored.
  /// @param field Field
  /// @return Value of the given field.
  [[nodiscard]] OptionalString read(
      const std::pair<StringView, StringView>& var) override;

  /// @brief Updating the value if it existed before.
  /// @param var pair with first - key where the hash is stored, second - field.
  /// @param dataField pair with first - data, second - empty string.
  /// @return true if it existed before, otherwise false.
  bool update(const std::pair<StringView, StringView>& var,
              const std::pair<StringView, StringView>& dataField) override;

  /// @brief Remove the given field from hash.
  /// @param key Key where the hash is stored.
  /// @param field Field to be removed.
  void del(const std::pair<StringView, StringView>& var) override;

  /// @brief Search by the specified nfType.
  /// @param nfTypeSearch nfType.
  /// @return Found keys.
  std::vector<OptionalString> find(const StringView& nfTypeSearch) override;

  /// @brief Сreating a database of a certain size.
  /// @param config_path The path to the template.
  /// @param n Size.
  void createUniDB(const char* config_path, const size_t n);

  RedisConnection redis_;
  std::shared_ptr<sw::redis::Redis> connection_{nullptr};
};

struct CreateRedis {
  CreateRedis(const std::string_view& key, const std::string_view& field,
              const std::string_view& dataField)
      : key_{key}, field_{field}, dataField_{dataField} {}
  CreateRedis() = delete;
  ~CreateRedis() = default;

  StringView key_;
  StringView field_;
  StringView dataField_;
};
