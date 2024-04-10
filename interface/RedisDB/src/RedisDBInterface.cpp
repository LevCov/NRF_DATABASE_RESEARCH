#include "../include/RedisDBInterface.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "../../UUID/UUIDv4.h"
#include "../nfTypes.h"

using json = nlohmann::json;

bool RedisDBInterface::create(const CreateRedis &var) {
  connection_->hset(var.key_, var.field_, var.dataField_);
  return true;
}

OptionalString RedisDBInterface::read(
    const std::pair<StringView, StringView> &var) {
  return connection_->hget(var.first, var.second);
}

bool RedisDBInterface::update(
    const std::pair<StringView, StringView> &var,
    const std::pair<StringView, StringView> &dataField) {
  if (!connection_->hget(var.first, var.second)) {
    connection_->hset(var.first, var.second, dataField.first);
    return true;
  }
  return false;
}

void RedisDBInterface::del(const std::pair<StringView, StringView> &var) {
  connection_->hdel(var.first, var.second);
}

std::vector<OptionalString> RedisDBInterface::find(
    const StringView &nfTypeSearch) {
  std::vector<OptionalString> match_keys;
  std::vector<OptionalString> keys;

  connection_->keys("*", std::back_inserter(keys));
  for (const auto &key : keys) {
    if (connection_->hget(*key, "nfType") == nfTypeSearch)
      match_keys.push_back(key);
  }

  return match_keys;
}

// creating DB with uniform dist nfprofiles. 14064 byte or ~ 0,014 Mb per key.
void RedisDBInterface::createUniDB(const char *config_path, const size_t n) {
  std::ifstream cf(config_path);

  if (cf.is_open()) {
    std::mt19937 rng;
    const uint8_t seed = 42;
    rng.seed(seed);
    std::uniform_int_distribution<int> dist(0, 20);
    json instances = json::parse(cf);

    for (auto &profile : instances) {  // for 1 of 1.
      for (size_t i = 0; i < n; ++i) {
        const std::string nfInstanceId = std::to_string(i);
        // std::string nfInstanceId = UUID::New().String();
        const uint8_t randomNumber = dist(rng);
        profile["nfInstanceId"] = nfInstanceId;
        profile["nfType"] = nfTypes[randomNumber];
        connection_->hset(nfInstanceId, "nfType", nfTypes[randomNumber]);
        connection_->hset(nfInstanceId, "data", profile.dump());
      }
    }

    cf.close();
  }
}
