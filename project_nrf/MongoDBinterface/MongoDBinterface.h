
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

#include <string> 


//Класс реализующий CRUD интерфейс для базы данных MongoDB

class MongoDBConnector {
public:

    //Конструктор задает uri для подлкючения к базе данных
    MongoDBConnector(const char* uri_string) {
     
        client_ = mongocxx::client{ mongocxx::uri{uri_string} };
        
    };

    //Метод Create создает базу данных и колеекцию
   // Принимает на вход имя новой базы данных и имя коллекции
    void Create(const char* database_name, const char* collection_name) {

       // mongocxx::instance instance{};

        try {

            client_ = mongocxx::client{ mongocxx::uri{"mongodb://localhost:27017"} };
            mongocxx::database db = client_[database_name];

            // Создаем коллекцию 
            mongocxx::collection coll = db[collection_name];

            bsoncxx::document::value doc_value = bsoncxx::from_json("{\"key\": \"value\"}");
            coll.insert_one(doc_value.view());
            std::cout << std::endl<<"DB created!" << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

    }


    // Метод Drop удаляет базу данных
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

    // Метод UpdateDocument обновляет документ в коллекцию конкретной базы данных.
   // Принимает на вход имя базы данных, имя коллекции, фильтр для поиска документа и новое ключ - значение для обновления
    void UpdateDocument(const char* database_name, const char* collection_name, const bsoncxx::document::view_or_value& filter, const bsoncxx::document::view_or_value& update) {
        
        
        //mongocxx::instance instance{};
        
        auto db = client_[database_name]; 
        auto collection = db[collection_name];
        collection.update_one(filter, update);
    }
   // Метод DeleteeDocument удаляет документ в коллекции конкретной базы данных.
   // Принимает на вход имя базы данных, имя коллекции, фильтр для поиска документа
     void DeleteDocument(const char* database_name, const char* collection_name, const bsoncxx::document::view_or_value& filter) {

     //mongocxx::instance instance{};

     auto db = client_[database_name];
     auto collection = db[collection_name];
     collection.delete_one(filter);

     bsoncxx::stdx::optional<bsoncxx::document::value> result = collection.find_one(filter);

     if (result) {
         std::cout << "Doc not deleted." << std::endl;
         //std::cout << bsoncxx::to_json(result.value()) << std::endl; // Вывести найденный документ
     }
     else {
         std::cout << "Doc deleted." << std::endl;
     }
 }
// Метод DeleteeDocument ищет документ в коллекции конкретной базы данных.
// Принимает на вход имя базы данных, имя коллекции, фильтр для поиска документа
//Возвращает возвращает документ в виде объекта 
auto FindDocument(const char* database_name, const char* collection_name, const bsoncxx::document::view_or_value& filter) {

    //mongocxx::instance instance{};

    auto db = client_[database_name];
    auto collection = db[collection_name];

    bsoncxx::stdx::optional<bsoncxx::document::value> result = collection.find_one(filter);
    
    if (result) {

        std::cout << "doc fnded";

        return result;

    }
    else {
        std::cout << "doc not fnded";
    }
    
}



private:
    mongocxx::client client_;
    mongocxx::uri uri;
    std::string dbname;
    std::string colname;
};


