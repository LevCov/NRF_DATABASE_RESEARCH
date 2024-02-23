#pragma once

#include <../include/connection.h>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

class MongoConn: public BDConnection<mongocxx::client>{
public:
    MongoConn(const char* uri);
    std::shared_ptr<mongocxx::client> connection() const;
private:
    std::shared_ptr<mongocxx::client> m_connection;
};