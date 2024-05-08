#pragma once

#include <libcouchbase/couchbase.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <random>
#include <string>
#include <utility>s
#include <vector>

#include "../../RedisDB/nfTypes.h"
#include "../../include/DBInterface.h"

void getCallback(lcb_INSTANCE *, int, const lcb_RESPGET *resp);
void queryCallback(lcb_INSTANCE *, int, const lcb_RESPQUERY *resp);
void ixmgmtCallback(__unused lcb_INSTANCE *instance_, __unused int cbtype,
                    const struct lcb_RESPN1XMGMT_st *resp);
void flushCallback(lcb_INSTANCE *, int, const lcb_RESPHTTP *resp);
void check(lcb_STATUS err, const char *msg);

struct Result_ {
  Result_() : value{""}, status{LCB_SUCCESS} {}
  std::string value;
  lcb_STATUS status{LCB_SUCCESS};
};

struct Result_u {
  std::string value{};
  std::uint64_t cas{0};
  lcb_STATUS rc{LCB_SUCCESS};
};

struct Rows {
  std::vector<std::string> rows{};
  std::string metadata{""};
};

class CouchBaseInterface final
    : public DBInterface<std::pair<std::string_view, std::string_view>, void,
                         std::string_view, std::string_view,
                         std::pair<std::string_view, std::string_view>, void> {
 public:
  using key_t = std::string_view;
  using value_t = std::string_view;

  CouchBaseInterface();
  ~CouchBaseInterface() = default;

  /// @brief Adds a new key value pair to the bucket.
  /// @param var Where first is key, second is value.
  /// @return Always true.
  bool create(const std::pair<key_t, value_t> &var) override;

  /// @brief Get the value of the given key in the bucket.
  /// @param key Key.
  void read(const key_t &key) override;

  /// @brief Update the value of the given key in the bucket.
  /// @param key Key.
  /// @param value Value.
  bool update(const key_t &key, const value_t &value) override;

  /// @brief Remove the given key-data pair from bucket.
  /// @param key Key.
  void del(const key_t &key) override;

  /// @brief Сreating DB with uniform dist nfprofiles.
  /// @param config_path Path to file with template.
  /// @param field Size of database.
  void createUniDB(const char *config_path, const size_t n);

  /// @brief Performs a bucket search for this nfType.
  /// @param var Where first is bucket name, second is nfType.
  void find(const std::pair<key_t, value_t> &var) override;

  /// @brief Flush a database.
  void flushdb();

  Result_ result_;
  Rows resultRows_;
  lcb_INSTANCE *instance_{nullptr};
};
