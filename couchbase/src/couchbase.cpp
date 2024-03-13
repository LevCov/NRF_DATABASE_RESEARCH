#include "../include/couchbase.h"

#include <libcouchbase/couchbase.h>

#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "../../redisdb/include/nfTypes.h"

/**
 ** \brief Represents a interface and search interactions in the redis database.
 */

using json = nlohmann::json;

void CouchBaseInterface::create(lcb_INSTANCE *instance, const std::string key,
                                const std::string value) {
  lcb_CMDSTORE *cmd = nullptr;
  check(lcb_cmdstore_create(&cmd, LCB_STORE_UPSERT), "create UPSERT command");
  check(lcb_cmdstore_key(cmd, key.c_str(), key.size()),
        "assign ID for UPSERT command");
  check(lcb_cmdstore_value(cmd, value.c_str(), value.size()),
        "assign value for UPSERT command");
  check(lcb_store(instance, nullptr, cmd), "schedule UPSERT command");
  check(lcb_cmdstore_destroy(cmd), "destroy UPSERT command");
  lcb_wait(instance, LCB_WAIT_DEFAULT);
}

Result_ CouchBaseInterface::read(lcb_INSTANCE *instance,
                                 const std::string key) {
  Result_ result{};

  lcb_CMDGET *cmd = nullptr;
  check(lcb_cmdget_create(&cmd), "create GET command");
  check(lcb_cmdget_key(cmd, key.c_str(), key.size()),
        "assign ID for GET command");
  check(lcb_get(instance, &result, cmd), "schedule GET command");
  check(lcb_cmdget_destroy(cmd), "destroy GET command");
  lcb_wait(instance, LCB_WAIT_DEFAULT);

  return result;
}

void CouchBaseInterface::update(lcb_INSTANCE *instance, const std::string key,
                                const std::string value) {
  Result_u result{};
  lcb_CMDSTORE *cmd = nullptr;
  check(lcb_cmdstore_create(&cmd, LCB_STORE_REPLACE), "create REPLACE command");
  check(lcb_cmdstore_key(cmd, key.c_str(), key.size()),
        "assign ID for REPLACE command");
  check(lcb_cmdstore_value(cmd, value.c_str(), value.size()),
        "assign value for REPLACE command");
  check(lcb_store(instance, &result, cmd), "schedule REPLACE command");
  check(lcb_cmdstore_destroy(cmd), "destroy UPSERT command");
  lcb_wait(instance, LCB_WAIT_DEFAULT);

  if (result.rc != LCB_SUCCESS) {
    std::cout << "failed to append " << ": " << lcb_strerror_short(result.rc)
              << "\n";
  }
}

void CouchBaseInterface::del(lcb_INSTANCE *instance, const std::string key) {
  lcb_CMDREMOVE *cmd = nullptr;
  check(lcb_cmdremove_create(&cmd), "create REMOVE command");
  check(lcb_cmdremove_key(cmd, key.c_str(), key.size()),
        "assign key for REMOVE command");
  check(lcb_remove(instance, nullptr, cmd), "schedule REMOVE command");
  check(lcb_cmdremove_destroy(cmd), "destroy REMOVE command");
  lcb_wait(instance, LCB_WAIT_DEFAULT);
}

void CouchBaseInterface::createUniDB(const char *config_path, int n,
                                     lcb_INSTANCE *instance) {
  std::ifstream cf(config_path);
  if (cf.is_open()) {
    std::mt19937 rng;
    const unsigned int seed = 42;
    rng.seed(seed);
    std::uniform_int_distribution<int> dist(0, 21);

    json instances = json::parse(cf);
    for (auto &profile : instances) {  // for 1 of 1.
      for (int i = 0; i < n; ++i) {
        const std::string nfInstanceId = std::to_string(i);
        const int randomNumber = dist(rng);
        profile["nfInstanceId"] = nfInstanceId;
        profile["nfType"] = nfTypes[randomNumber];
        (*this).create(instance, nfInstanceId, profile.dump());
      }
    }
    cf.close();
  }
}