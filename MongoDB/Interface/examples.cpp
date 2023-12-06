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


//Считывание файла для записи в базу данных.
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

connector.InsertDocument(database_name, collection_name, doc);  

//-------------------------------------------------/
//Пример как пользоваться методом UpdateOne();

//Создание фильтра для поиска документа. 
bsoncxx::builder::stream::document filter{};
filter << "key" << "value";


//Создание документа где где определено ключ - новое значение.
bsoncxx::builder::stream::document update{};
update << "$set" << bsoncxx::builder::stream::open_document << "key" << "new_value" << bsoncxx::builder::stream::close_document;

    
connector.UpdateOne(database_name, collection_name, filter.view(), update.view());  


  
//-------------------------------------------------/
//Пример как пользоваться методом UpdateMany();

//Создание фильтра для поиска документа. 
bsoncxx::builder::stream::document filter{};
filter << "key" << "value";


//Создание документа где где определено ключ - новое значение.
bsoncxx::builder::stream::document update{};
update << "$set" << bsoncxx::builder::stream::open_document << "key" << "new_value" << bsoncxx::builder::stream::close_document;

    
connector.UpdateMany(database_name, collection_name, filter.view(), update.view());  

//-------------------------------------------------/
//Пример как пользоваться методом DeleteDocument();

//Создание фильтра для поиска документа. 
bsoncxx::builder::stream::document filter{};
filter << "key" << "value";


connector.DeleteDocument(database_name, collection_name, filter.view());
    
//-------------------------------------------------/
//Пример как пользоваться методом FindDocument();

//Создание фильтра для поиска документа.  
bsoncxx::builder::stream::document filter{};
filter << "key"<< "value";    

//Создание переменной где будет храниться результат выполнения метода  FindDocument().
bsoncxx::stdx::optional<bsoncxx::document::value> result

//Выполнение метода FindDocument().

result = connector.FindDocument(database_name, collection_name, filter.view());  
  

//-------------------------------------------------/





  
  
}
