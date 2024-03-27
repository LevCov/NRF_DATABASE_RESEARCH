#include <iostream> 
#include <fstream> 
#include <string> 
#include <mysql_connection.h> 
#include <mysql_driver.h> 
#include <cppconn/statement.h> 
#include <cppconn/prepared_statement.h> 
#include <rapidjson/document.h> 
#include <ctime> 
#include <string>
#include "UUIDv4.h"

using namespace std;
using namespace rapidjson;


class MYSQLInterface
{
private:
    std::string tcp; // адрес
    std::string root; // имя пользователя
    std::string DBName;
    std::string password;

    sql::Connection* con;
    sql::Statement* stmt;
    sql::ResultSet* res;

public:

  //Конструктор интерфейся MYSQL
	MYSQLInterface(std::string tcp_, std::string root_, std::string password_, std::string DBName_)
	{
        tcp = tcp_;
        root = root_;
        password = password_;
        DBName = DBName_;

		sql::mysql::MySQL_Driver* driver; //инициализация драйвера

		driver = sql::mysql::get_mysql_driver_instance();
		con = driver->connect(tcp, root, password); //Подключение к базе данных
		con->setSchema(DBName);
        stmt = con->createStatement();
	}
	

  //Метод создания таблицы
  //Принимает на вход модель данных для заполнения имен полей и имя таблицы
	bool createTable(std::string path, std::string name)
	{
        ifstream ifs(path);
        string jsonData((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));

        // Парсинг JSON 
        Document document;
        document.Parse(jsonData.c_str());

        string createTableQuery = "CREATE TABLE IF NOT EXISTS " + name + " (";
        
        for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
             createTableQuery += "`" + string(itr->name.GetString()) + "` VARCHAR(36), "; // Изменить тип столбца на TEXT
        }
        createTableQuery += "id INT AUTO_INCREMENT PRIMARY KEY)"; // Добавим первичный ключ
       // createTableQuery.erase(createTableQuery.size() - 2, 2);
        //createTableQuery += " )";
        //std::cout << createTableQuery;
        try {
            stmt->execute(createTableQuery);
        }
        catch (sql::SQLException& e) {
            //cerr << "SQL Error: " << e.what() << endl;
        }

        string fillTableQuery;
        for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
            fillTableQuery = "INSERT INTO " + name + " (" + string(itr->name.GetString()) +  ")" + " VALUES " + "(" + " 'string' " + ")";
           // std::cout << fillTableQuery << std::endl;
            try {
                stmt->execute(fillTableQuery);
            }
            catch (sql::SQLException& e) {
                cerr << "SQL Error: " << e.what() << endl;
            }
        }
        return true;
	}

    //Метод чтения из таблицы
    //Принимает на вход имя таблицы и поле в котором нужно прочитать значение
    auto read(std::string table_name, std::string column_name) {
     
        
       //std::string ReadQuery = "SELECT " + column_name + " FROM " + table_name + " WHERE id = 1;";
        std::string ReadQuery = "SELECT * FROM " + table_name + " WHERE id = 1;";

        try {
            res = stmt->executeQuery(ReadQuery);
            //std::cout << "Column1: " << res->getString(column_name);
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        while (res->next()) {
            // Выводим только первую колонку
            std::cout << column_name + " : " << res->getString(1) << std::endl;
        }

        return res;
    }
    
    //Метод поиска конкректного значения в в кокнретном поле
    //Принимает на вход имя столбца и искомое значение
    auto find(std::string column_name, std::string searchValue) {

            // Создание временной таблицы для результатов поиска
            std::unique_ptr<sql::Statement> stmt(con->createStatement());
            stmt->execute(
                "CREATE TEMPORARY TABLE search_results (table_name VARCHAR(255), "
                "column_name VARCHAR(255), found_value TEXT)");

            // Получение списка всех таблиц
            std::unique_ptr<sql::ResultSet> tablesResult(
                stmt->executeQuery("SHOW TABLES"));

            while (tablesResult->next()) {
                std::string tableName = tablesResult->getString(1);

                // Получение списка всех столбцов в текущей таблице
                std::unique_ptr<sql::ResultSet> columnsResult(
                    stmt->executeQuery("SHOW COLUMNS FROM " + tableName));

                while (columnsResult->next()) {
                    std::string columnName = columnsResult->getString("Field");

                    // Генерация и выполнение SQL-запроса поиска
                    std::string sql =
                        "INSERT INTO search_results (table_name, column_name, "
                        "found_value) SELECT '" +
                        tableName + "', '" + columnName + "', " + columnName + " FROM " +
                        tableName + " WHERE " + columnName + " LIKE '%" + searchValue +
                        "%'";
                    stmt->execute(sql);
                }
            }

            // Вывод результатов поиска
            std::unique_ptr<sql::ResultSet> searchResults(
                stmt->executeQuery("SELECT * FROM search_results"));
            
            
            // Удаление временной таблицы
            stmt->execute("DROP TEMPORARY TABLE IF EXISTS search_results");


            return searchResults;
        
        

    }






    //Метод обновления данных в таблице
    //Принимает на вход имя таблицы, имя поля в котором нужно обновить данные и новое значение
    bool update(std::string table_name, std::string column_name, std::string new_value)
    {   
        std::string UpdateQuery = "UPDATE " +  table_name + " SET " + column_name + " = '" + new_value + "'" + " WHERE id = 1;";
        try {
            stmt->execute(UpdateQuery);
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        return true;
    }

    //Метод удаления таблицы
    bool del(std::string table_name)
    {
        std::string DeleteQuery = "DROP TABLE " + table_name;
        try {
            stmt->execute(DeleteQuery);
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        return true;
    }


    //Метод создания уникальной таблицы
    //Принимает на вход модель данных и количество уникальных таблиц, которые нужно создать.
    void createUniDB(std::string path, int n) {
        

        const char* nfTypes[]{ "NRF",    "UDM",  "AMF",  "SMF",   "AUSF", "NEF",
                         "PCF",    "SMSF", "NSSF", "UDR",   "LMF",  "GMLC",
                         "5G_EIR", "SEPP", "UPF",  "N3IWF", "AF",   "UDSF",
                         "BSF",    "CHF",  "NWDAF" };  // 20 nftypes.

        for (int i = 0; i < n; i++) {


            std::mt19937 rng;
            const unsigned int seed = i;
            rng.seed(seed);
            std::uniform_int_distribution<int> dist1(0, 20);
            const int randomNumber = dist1(rng);


            std::string name = "NRF_" + to_string(i);
            createTable(path, name);
            std::string uuid = UUID::New().String();
            update(name, "nfInstanceId", uuid);
            update(name, "nfType", nfTypes[randomNumber]);

        }

       

    }


    //Метод удаления задаваемого числа таблиц, созданных с помощью метода  createUniDB()
    void dropUniDB(int n) {

        for (int i = 0; i < n; i++) {

            std::string name = "NRF_" + to_string(i);
            del(name);
        }

    }



    //Деструктор
    ~MYSQLInterface() {
        delete stmt;
        delete con;
    }


};



