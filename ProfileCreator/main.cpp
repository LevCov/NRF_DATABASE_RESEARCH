#include "MongoDBConnector.h"
#include "ProfileCreator.h"





int main() {

    
  
  

    //инициализация драйвера
    mongocxx::instance instance{};
    //инициализация и подключение базы данных
    MongoDBConnector connector{ "mongodb://localhost:27017" };


    const char* database_name = "DBName";
    const char* collection_name = "ColName";


    //Создание 10 различных профилей для дальнейшего тестирования производительности базы данных
    N = 10;
    for (int i = 0; i < N;i++) {
        auto doc = bsoncxx::from_json(CreateProfile().GetString()); // Возвращаемое значение CreateProfile() преобразуется в string.
        connector.InsertDocument(database_name, collection_name, doc);
 
    }

    return 0;

}
