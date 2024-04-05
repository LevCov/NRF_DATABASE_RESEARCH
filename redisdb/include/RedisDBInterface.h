#pragma once

#include <vector>

#include "../include/DBInterface.h"
#include "redis_conn.h"

using OptionalString = sw::redis::OptionalString;
using StringView = sw::redis::StringView;

class RedisInterface
    : public DBInterface<std::pair<StringView, StringView>, OptionalString,
                         StringView, StringView, StringView,
                         std::vector<OptionalString>> {
 public:
  RedisInterface(const char* uri) : redis(uri) { conn = redis.connection(); }

  bool create(const std::pair<StringView, StringView>& key_val);

  OptionalString read(const StringView& key);

  bool update(const StringView& filter, const StringView& update);

  void del(const StringView& key);

  bool hcreate(const StringView& key, const StringView& field,
               const StringView& dataField);

  OptionalString hread(const StringView& key, const StringView& field);

  bool hupdate(const StringView& key, const StringView& field,
               const StringView& dataField);

  long long hdel(const StringView& key, const StringView& field);

  void createUniDB(const char* config_path, int n);

  std::vector<OptionalString> find(const StringView& nfTypeSearch);

  RedisConnection redis;
  std::shared_ptr<sw::redis::Redis> conn;
};
