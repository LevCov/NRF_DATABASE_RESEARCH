#include "../include/redis_conn.h"

explicit RedisConnection::RedisConnection(const char* m_uri)
    : connection_{std::make_shared<sw::redis::Redis>(m_uri)} {}

std::shared_ptr<sw::redis::Redis> RedisConnection::connection() const {
  return connection_;
}
