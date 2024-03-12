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
using namespace std;
using namespace rapidjson;


class MYSQLInterface
{
private:
    std::string tcp;
    std::string root;
    std::string DBName;
    std::string password;

    sql::Connection* con;
    sql::Statement* stmt;
    sql::ResultSet* res;

public:
	MYSQLInterface(std::string tcp_, std::string root_, std::string password_, std::string DBName_)
	{
        tcp = tcp_;
        root = root_;
        password = password_;
        DBName = DBName_;

		sql::mysql::MySQL_Driver* driver;
		driver = sql::mysql::get_mysql_driver_instance();
		con = driver->connect(tcp, root, password);
		con->setSchema(DBName);
        stmt = con->createStatement();
	}
	
	bool createTable(std::string path, std::string name)
	{
        ifstream ifs(path);
        string jsonData((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));

        // Парсинг JSON 
        Document document;
        document.Parse(jsonData.c_str());

        string createTableQuery = "CREATE TABLE IF NOT EXISTS " + name + " (";
        
        for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
            createTableQuery += "`" + string(itr->name.GetString()) + "` VARCHAR(20), "; // Изменить тип столбца на TEXT
        }
        createTableQuery += "id INT AUTO_INCREMENT PRIMARY KEY)"; // Добавим первичный ключ
       // createTableQuery.erase(createTableQuery.size() - 2, 2);
        //createTableQuery += " )";
        //std::cout << createTableQuery;
        try {
            stmt->execute(createTableQuery);
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }

        string fillTableQuery;
        for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
            fillTableQuery = "INSERT INTO " + name + " (" + string(itr->name.GetString()) +  ")" + " VALUES " + "(" + " 'string' " + ")";
            std::cout << fillTableQuery << std::endl;
            try {
                stmt->execute(fillTableQuery);
            }
            catch (sql::SQLException& e) {
                cerr << "SQL Error: " << e.what() << endl;
            }
        }
        return true;
	}


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





    ~MYSQLInterface() {
        delete stmt;
        delete con;
    }


};



