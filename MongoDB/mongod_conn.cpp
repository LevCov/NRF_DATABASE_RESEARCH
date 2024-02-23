#include "mongod_conn.h"

MongoConn::MongoConn(const char* m_uri){
    mongocxx::instance instance{};
    mongocxx::uri uri(m_uri);
    m_connection.reset(new mongocxx::client(uri));
}

std::shared_ptr<mongocxx::client> MongoConn::connection() const {
    return m_connection;
}