#pragma once

#include <libcouchbase/couchbase.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "../../../redisdb/include/nfTypes.h"
#include "../../include/DBInterface.h"

void getCallback(lcb_INSTANCE *, int, const lcb_RESPGET *resp);
void queryCallback(lcb_INSTANCE *, int, const lcb_RESPQUERY *resp);
void ixmgmtCallback(__unused lcb_INSTANCE *instance_, __unused int cbtype,
                    const struct lcb_RESPN1XMGMT_st *resp);
void httpCallback(lcb_INSTANCE *, int, const lcb_RESPHTTP *resp);
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

class CouchBaseInterface
    : public DBInterface<std::pair<std::string, std::string>, void, std::string,
                         std::string, std::pair<std::string, std::string>,
                         void> {
 public:
  CouchBaseInterface();
  ~CouchBaseInterface();

  /// @brief Adds a new key value pair to the bucket.
  /// @param instance_ lcb_INSTANCE(couchbase) instance_.
  /// @param key Key.
  /// @param value Value.
  bool create(const std::pair<std::string, std::string> &var) override;

  void find(const std::pair<std::string, std::string> &var) override;

  /// @brief Get the value of the given key in the bucket.
  /// @param instance_ lcb_INSTANCE(couchbase) instance_.
  /// @param key Key.
  /// @return Cookie with result.
  void read(const std::string &key) override;

  /// @brief Update the value of the given key in the bucket.
  /// @param instance_ lcb_INSTANCE(couchbase) instance_.
  /// @param key Key.
  /// @param value Value.
  bool update(const std::string &key, const std::string &value) override;

  /// @brief Remove the given key-data pair from bucket.
  /// @param instance_ lcb_INSTANCE(couchbase) instance_.
  /// @param key Key.
  void del(const std::string &key) override;

  /// @brief Сreating DB with uniform dist nfprofiles.
  /// @param config_path Path to file with template.
  /// @param field Size of database.
  void createUniDB(const char *config_path, const size_t n);

  void flushdb();

  lcb_INSTANCE *instance_{nullptr};
  Result_ result_;
  Rows resultRows_;
};
