#pragma once

#include <libcouchbase/couchbase.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

struct Result_ {
  std::string value{};
  lcb_STATUS status{LCB_SUCCESS};
};

struct Result_u {
  std::string value{};
  std::uint64_t cas{0};
  lcb_STATUS rc{LCB_SUCCESS};
};

class CouchBaseInterface {
 public:
  /// @brief Adds a new key value pair to the bucket.
  /// @param instance lcb_INSTANCE(couchbase) instance.
  /// @param key Key.
  /// @param value Value.
  void create(lcb_INSTANCE *instance, const std::string key,
              const std::string value);

  /// @brief Get the value of the given key in the bucket.
  /// @param instance lcb_INSTANCE(couchbase) instance.
  /// @param key Key.
  /// @return Cookie with result.
  Result_ read(lcb_INSTANCE *instance, const std::string key);

  /// @brief Update the value of the given key in the bucket.
  /// @param instance lcb_INSTANCE(couchbase) instance.
  /// @param key Key.
  /// @param value Value.
  void update(lcb_INSTANCE *instance, const std::string key,
              const std::string value);

  /// @brief Remove the given key-data pair from bucket.
  /// @param instance lcb_INSTANCE(couchbase) instance.
  /// @param key Key.
  void del(lcb_INSTANCE *instance, const std::string key);

  /// @brief Сreating DB with uniform dist nfprofiles.
  /// @param config_path Path to file with template.
  /// @param field Size of database.
  void createUniDB(const char *config_path, int n, lcb_INSTANCE *instance);

  static void check(lcb_STATUS err, const char *msg) {
    if (err != LCB_SUCCESS) {
      std::cerr << "[ERROR] " << msg << ": " << lcb_strerror_short(err) << "\n";
      exit(EXIT_FAILURE);
    }
  }
};