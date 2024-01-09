**Демонстрация работы методов интерфеса**

1. Метод Create();

   код:
   
   int main()
   {
   
    //инициализация драйвера
    mongocxx::instance instance{};
   
    //инициализация и подключение базы данных
    MongoDBConnector connector{ "mongodb://localhost:27017" };

    const char* database_name = "DBName";
    const char* collection_name = "ColName";

    connector.Create(database_name, collection_name);

   }

Итог работы:
![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/d42d80ce-6742-4408-83b4-915e31a5cd99)

Также появилась новая база данных с коллецией.

![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/e82bc4f7-97b3-47e7-b6f0-c2ec4fcbb507)







2. Метод Drop();

  код:

int main() 
{

    //инициализация драйвера
    mongocxx::instance instance{};

    //инициализация и подключение базы данных
    MongoDBConnector connector{ "mongodb://localhost:27017" };

    const char* database_name = "DBName";
    const char* collection_name = "ColName";

    connector.Drop(database_name);

}

   Итог работы:
    ![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/31dd2fa1-ea7d-41df-a835-deeb1ec0745c)



3. Метод InsertDocument();

   код:

int main() {


    //инициализация драйвера
    mongocxx::instance instance{};


    //инициализация и подключение базы данных
    MongoDBConnector connector{ "mongodb://localhost:27017" };

    const char* database_name = "DBName";
    const char* collection_name = "ColName";

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
        
    }

    // Разбираем JSON строку
    auto doc = bsoncxx::from_json(jsonContent);

    connector.InsertDocument(database_name, collection_name, doc);

    return 0;

}


   Итог работы:

   ![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/8b705bc3-14bd-4d89-8cfb-3e24027b5a23)

   В базе данных появился документ data_model.json


4. Метод UpdateOne();


   код:

int main() {


    //инициализация драйвера
    mongocxx::instance instance{};

    //инициализация и подключение базы данных
    MongoDBConnector connector{ "mongodb://localhost:27017" };


    const char* database_name = "DBName";
    const char* collection_name = "ColName";

  
    //Создание фильтра для поиска документа. 
    bsoncxx::builder::stream::document filter{};
    filter << "0.nfType" << "NRF";


    //Создание документа где где определено ключ - новое значение.
    bsoncxx::builder::stream::document update{};
    update << "$set" << bsoncxx::builder::stream::open_document << "0.nfType" << "Updated" << bsoncxx::builder::stream::close_document;


    connector.UpdateOne(database_name, collection_name, filter.view(), update.view());

    return 0;

}


   Итог работы:
   ![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/c98cf310-d73f-45d1-9810-0d0ec019da37)
   В базе данных значение в поле nfType поменялось на Updated.




5. Метод DeleteDocument();

    код:
   
   int main() {
   
    //инициализация драйвера
    mongocxx::instance instance{};


    //инициализация и подключение базы данных
    MongoDBConnector connector{ "mongodb://localhost:27017" };


    
    const char* database_name = "DBName";
    const char* collection_name = "ColName";

  
    //Создание фильтра для поиска документа. 
    bsoncxx::builder::stream::document filter{};
    filter << "0.nfType" << "Updated";

    connector.DeleteDocument(database_name, collection_name, filter.view());

    return 0;

}


 Итог работы:

 ![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/c12a3039-8beb-4b11-81df-8836f04a65b0)

  В базе данных удалился документ по заданному фильтру.

