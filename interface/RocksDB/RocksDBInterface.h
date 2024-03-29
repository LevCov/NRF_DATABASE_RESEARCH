#pragma once

#include "../include/DBInterface.h"
#include "CRUDparams.h"
#include "Rockspackage.h"

class RocksDBInterface: public DBInterface<Create,
 Read, Update, Del_Read, FindPar, FindRet>{
public:
    DB* conn;

    // вектор ColumnFamilyHandle (семейств столбцов)
    std::vector<ColumnFamilyHandle*> handles;

    // позиция nfType в векторе ColumnFamilyHandle
    std::unordered_map<std::string, int> NF = {
        {"NRF", 1}, {"UDM", 2}, {"AMF", 3}, {"SMF",4}, {"AUSF",5}, 
        {"NEF", 6}, {"PCF", 7}, {"SMSF", 8}, {"NSSF", 9}, {"UDR", 10}, 
        {"LMF", 11}, {"GMLC", 12}, {"5G_EIR", 13}, {"SEPP",14}, {"UPF", 15},
        {"N3IWF", 16}, {"AF", 17}, {"UDSF", 18}, {"BSF", 19}, {"CHF", 20}, 
        {"NWDAF", 21}
    };
public:
    RocksDBInterface(const char* url);
    ~RocksDBInterface();
    // главный метод поиска
    FindRet find(const FindPar& par);

    // создание базы из n NFProfile на основе json файла path
    void createUniDB(const char *path, int n);
    // очистка БД
    void flushdb();
public:
    //CRUD
    bool create(const Create& par);

    Read read(const Del_Read& par);
    
    bool update(const Update& filter, const Update& update);

    void del(const Del_Read& par);
};