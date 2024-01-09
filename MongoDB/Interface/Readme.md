**Демонстрация работы методов интерфеса**

1. Метод Create();

   код:
   
   int main() {


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

int main() {


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
   
