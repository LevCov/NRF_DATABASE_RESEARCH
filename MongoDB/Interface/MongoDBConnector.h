#include <iostream> 
#include <fstream> 
#include <mongocxx/client.hpp> 
#include <mongocxx/instance.hpp> 
#include <bsoncxx/builder/basic/document.hpp> 
#include <bsoncxx/builder/stream/document.hpp> 
#include <mongocxx/bulk_write.hpp>
#include <mongocxx/uri.hpp> 
#include <bsoncxx/json.hpp> 
#include <bsoncxx/types.hpp>
#include <bsoncxx/stdx/make_unique.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/document/view_or_value.hpp>
#include <typeinfo>
#include <string> 
 

class MongoDBConnector {
public:
    MongoDBConnector(const char* uri_string) {
     
        //Конструктор задает uri для подлкючения к базе данных 
        client_ = mongocxx::client{ mongocxx::uri{uri_string} };
        
    };



    //Метод Create() создает базу данных и коллекцию 
   // Принимает на вход имя новой базы данных и имя коллекции 

    void Create(const char* database_name, const char* collection_name) {

       

        try {

            client_ = mongocxx::client{ mongocxx::uri{"mongodb://localhost:27017"} };
            mongocxx::database db = client_[database_name];

            // Создаем коллекцию 
            mongocxx::collection coll = db[collection_name];
            int i = 0;
            bsoncxx::document::value doc_value = bsoncxx::from_json("{\"key\": \"value\"}");

            
            coll.insert_one(doc_value.view());
            
            


            std::cout << std::endl<<"DB " << database_name << " created!";
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

    }


    // Метод Drop() удаляет базу данных 
   // Принимает на вход имя базы данных 
    bool Drop(const char* database_name)
    {
       
        client_ = mongocxx::client{ mongocxx::uri{"mongodb://localhost:27017"} };
        try {
            client_.database(database_name).drop(); // Удаляем базу данных
            std::cout<<std::endl << "Database " << database_name << " deleted!" << std::endl;
            return true; // Успешно удалено
        }
        catch (const std::exception& e) {
            std::cerr << "Error whe deleting: " << e.what() << std::endl;
            return false; // Ошибка при удалении
        }
       
   
    }

     // Метод InsertDocument вставляет документ в коллекцию конкретной базы данных. 
   // Принимает на вход имя базы данных, имя коллекции и документ для вставки 
    void InsertDocument(const char* database_name, const char* collection_name, const bsoncxx::document::value& document) {

        //mongocxx::instance instance{};
        auto db = client_[database_name];
        auto collection = db[collection_name];
        collection.insert_one(document.view());

        
    }

    // Метод UpdateOne() обновляет один документ в коллекции базы данных. 
   // Принимает на вход имя базы данных, имя коллекции, фильтр для поиска документа и новое ключ - значение для обновления 
    void UpdateOne(const char* database_name, const char* collection_name, const bsoncxx::document::view_or_value& filter, const bsoncxx::document::view_or_value& update) {
        
        
        //mongocxx::instance instance{};
        
        auto db = client_[database_name]; 
        auto collection = db[collection_name];
        collection.update_one(filter, update);

        bsoncxx::stdx::optional<bsoncxx::document::value> result = collection.find_one(filter);

        /*
        if (result) {
            std::cout << "Value  updated" << std::endl;

        }
        else {
            std::cout << "Value not updated" << std::endl;
        }
        */
    }


     // Метод UpdateMnay() обновляет все документы соответствующие фильтру в коллекции базы данных. 
   // Принимает на вход имя базы данных, имя коллекции, фильтр для поиска документа и новое ключ - значение для обновления 
    void UpdateMany(const char* database_name, const char* collection_name, const bsoncxx::document::view_or_value& filter, const bsoncxx::document::view_or_value& update) {


        //mongocxx::instance instance{};

        auto db = client_[database_name];
        auto collection = db[collection_name];
        collection.update_many(filter, update);

        bsoncxx::stdx::optional<bsoncxx::document::value> result = collection.find_one(filter);

        /*
        if (result) {
            std::cout << "Value  updated" << std::endl;

        }
        else {
            std::cout << "Value not updated" << std::endl;
        }
        */
    }

    // Метод DeleteDocument() удаляет документ по заданному фильтру в коллекции  базы данных. 
   // Принимает на вход имя базы данных, имя коллекции, фильтр для поиска документа 
    void DeleteDocument(const char* database_name, const char* collection_name, const bsoncxx::document::view_or_value& filter) {

        //mongocxx::instance instance{};

        auto db = client_[database_name];
        auto collection = db[collection_name];
        collection.delete_one(filter);

        bsoncxx::stdx::optional<bsoncxx::document::value> result = collection.find_one(filter);
        /*
        if (result) {
            std::cout << "Doc deleted." << std::endl;
            
        }
        else {
            std::cout << "Doc not deleted." << std::endl;
        }
        */
    }


    // Метод FindDocument() ищет документ по фильтру в коллекции базы данных.
    // Принимает на вход имя базы данных, имя коллекции, фильтр для поиска документа.
    //Возвращает опциональное значение, которое может содержать документ (если он был найден) или быть пустым (если ничего не было найдено).

    auto FindDocument(const char* database_name, const char* collection_name, const bsoncxx::document::view_or_value& filter) {


        auto db = client_[database_name];
        auto collection = db[collection_name];

        bsoncxx::stdx::optional<bsoncxx::document::value> result = collection.find_one(filter);


        return result;

      
    }




private:
    mongocxx::client client_;
    mongocxx::uri uri;
    std::string dbname;
    std::string colname;
};


