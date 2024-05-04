#include "MYSQLInterface.h"

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

MYSQLInterface(std::string tcp_, std::string root_, std::string password_, std::string DBName_);

bool createTable(std::string path, std::string name);

auto read(std::string table_name, std::string column_name);

auto find(std::string column_name, std::string searchValue);

bool update(std::string table_name, std::string column_name, std::string new_value);

bool del(std::string table_name);

void createUniDB(std::string path, int n);

void dropUniDB(int n);

~MYSQLInterface();


};
