
#include "Interface.h"


int main() {
    
    std::string tcp = "tcp://localhost:3306";
    std::string root = "root";
    std::string password = "0000";
    std::string DBName = "test";
    std:string path = "data_model.json";


    MYSQLInterface a(tcp, root, password, DBName);
    
    //a.createTable(path,"NRF_5");
    //a.update("nrf_6", "nfInstanceId","23232");
    //a.read("nrf_6", "nfStatus");
    a.del("qq");


    return 0;
}
