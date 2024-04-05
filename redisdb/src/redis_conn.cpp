#include <redis_conn.h>

RedisConnection::RedisConnection(const char* uri) {
  connection_ = std::make_shared<Redis>(uri);
}

std::shared_ptr<Redis> RedisConnection::connection() const {
  return connection_;
}