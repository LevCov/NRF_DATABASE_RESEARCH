#pragma once

#include <iostream> 
#include <bsoncxx/builder/basic/document.hpp> 
#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/document/view_or_value.hpp>

#include <mongod_conn.h>
#include <../include/DBInterface.h>
 

class MongoDBInterface: public DBInterface<bsoncxx::document::view_or_value,
                                mongocxx::cursor,
                                bsoncxx::document::view_or_value,
                                bsoncxx::document::view_or_value,
                                bsoncxx::document::view_or_value,
                                mongocxx::cursor>{
public:
    MongoDBInterface(const char* uri): client(uri){};

    void set_db(const char* database);

    void set_collection(const char* collection);

    void drop();

    mongocxx::cursor find(const bsoncxx::document::view_or_value& filter);

    bsoncxx::document::value from_JSON(const char* path);

    void createUniDB(const char * config_path, int n);
    
public:
    bool create(const bsoncxx::document::view_or_value& document) override;

    mongocxx::cursor read(const bsoncxx::document::view_or_value& read) override;
    
    bool update(const bsoncxx::document::view_or_value& filter, const bsoncxx::document::view_or_value& update) override;

    void del(const bsoncxx::document::view_or_value& filter) override;

public:
    MongoConn client;
    mongocxx::database db;
    mongocxx::collection collect;
};
