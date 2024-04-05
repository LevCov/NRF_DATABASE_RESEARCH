#include "../include/redis_find.h"

using json = nlohmann::json;
using Redis = sw::redis::Redis;
using StringView = sw::redis::StringView;
using OptionalString = sw::redis::OptionalString;

int main() {
  auto redis = std::make_unique<RedisInterface>("tcp://127.0.0.1:6379");

  std::cout << "Hello, World!";

  return 0;
}
