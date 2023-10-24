#include "mongod_conn.h"
#include "config.h"

MongoConn::MongoConn(){
    mongocxx::instance instance{};
    mongocxx::uri uri(mongo_d::m_uri);
    m_connection.reset(new mongocxx::client(uri));
}

std::shared_ptr<mongocxx::client> MongoConn::connection() const {
    return m_connection;
}