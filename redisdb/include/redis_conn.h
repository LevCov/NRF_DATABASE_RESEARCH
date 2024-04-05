#pragma once

#include <hiredis/hiredis.h>
#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>

#include "../include/connection.h"

using Redis = sw::redis::Redis;

class RedisConnection : public BDConnection<Redis> {
 public:
  RedisConnection(const char* uri);
  std::shared_ptr<Redis> connection() const override;

 private:
  std::shared_ptr<Redis> connection_ = nullptr;
};