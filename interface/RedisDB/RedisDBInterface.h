#pragma once

#include <iostream> 

#include <redis_conn.h>
#include <../include/DBInterface.h>
#include <vector>

using OptStr = sw::redis::OptionalString;
using StrView = sw::redis::StringView;
 

class RedisDBInterface: public DBInterface<std::pair<StrView,StrView>, 
                        OptStr, 
                        StrView, 
                        StrView,
                        StrView,
                        std::vector<OptStr>>{
public:
    RedisConn redis;
    std::shared_ptr<sw::redis::Redis> conn;

public:
    RedisDBInterface(const char* uri): redis(uri){
        conn = redis.connection();
    }

    void createUniDB(const char * config_path, int n);
    std::vector<OptStr> find(const StrView &nfTypeSearch);
    
    bool hcreate(const StrView& key,
                 const StrView& field,
                 const StrView& dataField);
    OptStr hread(const StrView &key, const StrView &field);
    bool hupdate(const StrView& key,
                 const StrView& field,
                 const StrView& dataField);
    long long hdel(const StrView &key, const StrView &field);
    
public:
    bool create(const std::pair<StrView, StrView>& key_val);

    OptStr read(const StrView& key);
    
    bool update(const StrView& filter, const StrView& update);

    void del(const StrView& key);

};
