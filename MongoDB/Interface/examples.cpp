#include "MongoDBConnector.h"




int main(){

  
//инициализация драйвера
mongocxx::instance instance{};

  
//инициализация и подключение базы данных
MongoDBConnector connector{ "mongodb://localhost:27017" };


//Пример как можно задать имена базы данных и коллекции  
const char* database_name = "DBName";
const char* collection_name = "ColName";

//-------------------------------------------------/
//Пример как пользоваться методом Create()  
  
connector.Create(database_name1, collection_name);
  
//-------------------------------------------------/
//Пример как пользоваться методом Drop()

connector.Drop(database_name);

//-------------------------------------------------/
//Пример как пользоваться методом InsertDocument();


//c  
std::string jsonContent;
std::ifstream jsonFile("data_model.json");
if (jsonFile.is_open())
{
    std::getline(jsonFile, jsonContent, '\0');
    jsonFile.close();
}
else
{
    std::cerr << "Unable to open JSON file." << std::endl;
    return;
}

    // Разбираем JSON строку
    auto doc = bsoncxx::from_json(jsonContent);

  

/-------------------------------------------------/
/-------------------------------------------------/
/-------------------------------------------------/
/-------------------------------------------------/
/-------------------------------------------------/
/-------------------------------------------------/




  
  
}
