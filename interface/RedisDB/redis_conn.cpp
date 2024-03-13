#include "redis_conn.h"

RedisConn::RedisConn(const char* m_uri){
    m_connection = std::make_shared<sw::redis::Redis>(m_uri);
}

std::shared_ptr<sw::redis::Redis> RedisConn::connection() const {
    return m_connection;
}