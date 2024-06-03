#include "Interface.h"


int main() {
    
    std::string tcp = "tcp://localhost:3306";
    std::string root = "root";
    std::string password = "0000";
    std::string DBName = "test";
    std:string path = "data_model.json";


    MYSQLInterface a(tcp, root, password, DBName);


    //Пример создания таблицы
    a.createTable(path,"nrf_10");

    //Обновления значения nfInstanceId в созданной таблице
    a.update("nrf_10", "nfInstanceId","23232");
  
    //Чтение данных из таблицы
    a.read("nrf_10", "nfStatus");

    //Удаления таблицы
    a.del("nrf_10");

    //Создание 5 уникальных таблиц
    a.createUniDB(path, 5);
  
    //Поиск таблицы в которой встречается nfType - AMF
    a.find("nfType", "AMF");

    //Удаление всех уникальных таблиц
    a.dropUniDB(5);

    


    return 0;
}
