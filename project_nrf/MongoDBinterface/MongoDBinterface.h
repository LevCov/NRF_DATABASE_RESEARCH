#include <iostream>
#include <stdio.h>
#include <fstream>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <string>
#include <typeinfo>

class MongoDBConnector {
public:
    MongoDBConnector(const char *uri_string) {
 
        client_ = mongocxx::client{ mongocxx::uri{uri_string} };
        
    };
    

    void InsertDocument(const char* database_name, const char* collection_name, const bsoncxx::document::value& document) {
        
        mongocxx::instance instance{};
        auto db = client_[database_name];
        auto collection = db[collection_name];
        collection.insert_one(document.view());
    }

    auto FindDocuments (const char* database_name, const char* collection_name, const bsoncxx::document::view_or_value& filter) {
        
        
        mongocxx::instance instance{};

        auto db = client_[database_name];
        auto collection = db[collection_name];
        return collection.find(filter);

        
    }

    void UpdateDocument(const char* database_name, const char* collection_name, const bsoncxx::document::view_or_value& filter, const bsoncxx::document::view_or_value& update) {
       
        mongocxx::instance instance{};
        
        auto db = client_[database_name];
        auto collection = db[collection_name];
        collection.update_one(filter.view(), update.view());
    }

    void DeleteDocument(const char* database_name, const char* collection_name, const bsoncxx::document::view_or_value& filter) {
        
        mongocxx::instance instance{};
        
        auto db = client_[database_name];
        auto collection = db[collection_name];
        collection.delete_one(filter);
    }





private:
    mongocxx::client client_;
    mongocxx::uri uri;
    std::string dbname;
    std::string colname;
};
