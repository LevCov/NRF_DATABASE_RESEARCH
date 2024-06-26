#include "../include/RedisDBInterface.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "../../UUID/UUIDv4.h"
#include "../nfTypes.h"

namespace {
using json = nlohmann::json;
using value_t = OptionalString;
using keys_t = std::vector<OptionalString>;
}  // namespace

bool RedisDBInterface::create(const CreateRedis &var) {
  return connection_->hset(var.key_, var.field_, var.dataField_);
}

value_t RedisDBInterface::read(const keyField_t &var) {
  return connection_->hget(var.first, var.second);
}

bool RedisDBInterface::update(const keyField_t &var,
                              const keyField_t &dataField) {
  if (!connection_->hget(var.first, var.second)) {
    connection_->hset(var.first, var.second, dataField.first);
    return true;
  }
  return false;
}

void RedisDBInterface::del(const keyField_t &var) {
  connection_->hdel(var.first, var.second);
}

[[nodiscard]] keys_t RedisDBInterface::find(const FindRedis &var) {
  keys_t match_keys;
  keys_t keys;
  match_keys.reserve(var.n_ / (nfTypes.size() - 1));
  keys.reserve(var.n_);

  connection_->keys("*", std::back_inserter(keys));
  for (auto &&key : keys) {
    if (connection_->hget(*key, "nfType") == var.nfTypeSearch_) [[unlikely]]
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
    std::uniform_int_distribution<int> dist(0, nfTypes.size());
    json instances = json::parse(cf);

    for (auto &&profile : instances) {  // for 1 of 1.
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

void checkRedisConnection(const std::shared_ptr<sw::redis::Redis> &connection) {
  if (!connection.use_count())
    std::cerr << "Not connected to the redis database\n";
}
