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
struct FindRedis;

/// @brief Interface to the redis database.
class RedisDBInterface
    : public DBInterface<CreateRedis, OptionalString,
                         std::pair<StringView, StringView>,
                         std::pair<StringView, StringView>, FindRedis,
                         std::vector<OptionalString>> {
 public:
  using keyField_t = std::pair<StringView, StringView>;
  using value_t = OptionalString;
  using keys_t = std::vector<OptionalString>;

  /// @brief Redis initialization.
  /// @param uri The server at the address 127.0.0.1 with port 6379.
  explicit RedisDBInterface(const char* uri)
      : redis_(uri), connection_{redis_.connection()} {}

  RedisDBInterface() = delete;
  RedisDBInterface(const RedisDBInterface& rhs) = delete;
  RedisDBInterface& operator=(const RedisDBInterface& rhs) = delete;
  RedisDBInterface(RedisDBInterface&& rhs) = delete;
  RedisDBInterface&& operator=(RedisDBInterface&& rhs) = delete;
  ~RedisDBInterface() = default;

  /// @brief Creating a hash object in the redis database.
  /// @param var The parameter is a structure with a key where the hash is
  /// stored, field and value.
  /// @return Always true.
  bool create(const CreateRedis& var) final;

  /// @brief Reading a hash object from the redis database.
  /// @param key Key where the hash is stored.
  /// @param field Field
  /// @return Value of the given field.
  [[nodiscard]] value_t read(const keyField_t& var) final;

  /// @brief Updating the value if it existed before.
  /// @param var pair with first - key where the hash is stored, second - field.
  /// @param dataField pair with first - data, second - empty string.
  /// @return true if it existed before, otherwise false.
  bool update(const keyField_t& var, const keyField_t& dataField) final;

  /// @brief Remove the given field from hash.
  /// @param key Key where the hash is stored.
  /// @param field Field to be removed.
  void del(const keyField_t& var) final;

  /// @brief Search by the specified nfType.
  /// @param nfTypeSearch nfType.
  /// @return Found keys.
  keys_t find(const FindRedis& var) final;

  /// @brief Сreating a database of a certain size.
  /// @param config_path The path to the template.
  /// @param n Size.
  void createUniDB(const char* config_path, const size_t n);

  RedisConnection redis_;
  std::shared_ptr<sw::redis::Redis> connection_{nullptr};
};

void checkRedisConnection(const std::shared_ptr<sw::redis::Redis>& connection);

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

struct FindRedis {
  explicit FindRedis(const std::string_view& nfTypeSearch, const size_t n = 0)
      : nfTypeSearch_(nfTypeSearch), n_(n) {}
  FindRedis() = delete;
  ~FindRedis() = default;
  StringView nfTypeSearch_;
  size_t n_;
};
