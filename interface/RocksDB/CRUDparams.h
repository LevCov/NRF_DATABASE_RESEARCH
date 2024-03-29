#pragma once

#include "Rockspackage.h"
 
 // handle_n - номер ColumnFamily в соответствии с map NF
 // 0 - дефолтный ColumnFamily
struct Create 
{
    int handle_n;
    Slice key;
    Slice value;
};

struct Del_Read
{
    int handle_n;
    Slice key;
};

struct Read
{
    std::string value;
};

struct Update
{
    int handle_n;
    Slice key;
    Slice value;
};

struct FindPar 
{
    int handle_n;
};

struct FindRet
{
    std::vector<std::string> values;
};