#pragma


#include <unordered_map>
#include <string>
#include <MongoDB/mongod_conn.h>
#include <bsoncxx/builder/basic/document.hpp>
using std::string;

class Discovery{
public:
    void get(std::unordered_map<string, string> query){
        create_document_query(query);
    }
    void create_document_query(const auto& query){
        auto doc_value = make_do
        for(const auto& key-val: query){
        }
    }

private:
    Discovery(){
        auto client = connect.get()->connection();
        db = client.get()->["NFProfiles"];
    };

private:
    inline static auto connect = std::make_shared<MongoConn>();
    mongocxx::database db;
};