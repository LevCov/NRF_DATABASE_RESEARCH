#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;


class createMD{

    createMD(){
        mongocxx::uri uri("mongodb://localhost:27017");
        mongocxx::client client(uri);
        auto db = client["db"];
        auto coll = db["coll"];
        std::string path = "NFProfiles";
    
        for(const auto & entry : fs::directory_iterator(path)){
            std::ifstream json(entry.path());
            std::string str, result;

            while(json >> str){
                result.append(str);
            }
        
            bsoncxx::document::view_or_value doc = bsoncxx::from_json(result);
            auto insert_result = coll.insert_one(doc);
        }
        
    }
    ~createMD(){
        mongocxx::uri uri("mongodb://localhost:27017");
        mongocxx::client client(uri);
        client["db"].drop();
    }
};