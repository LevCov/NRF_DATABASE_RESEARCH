//===----------------------------------------------------------------------===//

#include "../include/redis_find.h" 

using json = nlohmann::json;
using Redis = sw::redis::Redis;
using StringView = sw::redis::StringView;
using OptionalString = sw::redis::OptionalString;

int main() {
    
    auto redis = std::make_unique<RedisInterface>("tcp://127.0.0.1:6379");

    try {

        auto v = redis->find("NRF");

        for (const auto& i : v) {
            std::cout << *i << " ";
            auto str = redis->hget(*i, "data");
            // std::cout << *str;
            // json profile = json::parse(*str);
            // std::cout << std::setw(4) << profile.flatten() << "\n\n";
        }

        redis->loadingDB("../data/data_model.json");
        // redis.flushdb();
        // redis.flushdb();

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
