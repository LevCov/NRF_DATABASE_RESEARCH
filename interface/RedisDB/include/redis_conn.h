#pragma once

#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>

#include <memory>

#include "../../include/connection.h"

class RedisConnection : public BDConnection<sw::redis::Redis> {
 public:
  /// @brief Redis initialization.
  /// @param uri The server at the address 127.0.0.1 with port 6379.
  explicit RedisConnection(const char* uri);
  RedisConnection() = delete;
  RedisConnection(const RedisConnection& rhs) = delete;
  RedisConnection& operator=(const RedisConnection& rhs) = delete;
  RedisConnection(const RedisConnection&& rhs) = delete;
  RedisConnection&& operator=(const RedisConnection&& rhs) = delete;
  ~RedisConnection() = default;

  /// @brief Redis initialization.
  /// @return Connection pointer.
  std::shared_ptr<sw::redis::Redis> connection() const override;

 private:
  std::shared_ptr<sw::redis::Redis> connection_;
};
