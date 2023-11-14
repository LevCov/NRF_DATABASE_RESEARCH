//===----------------------------------------------------------------------===//

#include "../include/redis_find.h" 

using json = nlohmann::json;
using Redis = sw::redis::Redis;
using StringView = sw::redis::StringView;
using OptionalString = sw::redis::OptionalString;

int main() {
    
    std::cout << "Hello, World!";

    try {
        auto redis = std::make_unique<RedisInterface>("tcp://127.0.0.1:6379");
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
