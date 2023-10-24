#pragma once

#include <../connection.h>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

class MongoConn: public BDConnection<mongocxx::client>{
public:
    MongoConn();
    std::shared_ptr<mongocxx::client> connection() const;
private:
    std::shared_ptr<mongocxx::client> m_connection;
};