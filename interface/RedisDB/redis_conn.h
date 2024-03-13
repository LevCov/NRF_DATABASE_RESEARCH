#pragma once

#include <../include/connection.h>
#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>
#include <hiredis/hiredis.h>

class RedisConn: public BDConnection<sw::redis::Redis>{
public:
    RedisConn(const char* uri);
    std::shared_ptr<sw::redis::Redis> connection() const;
private:
    std::shared_ptr<sw::redis::Redis> m_connection;
};