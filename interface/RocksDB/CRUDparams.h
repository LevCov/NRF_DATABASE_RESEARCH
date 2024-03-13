#pragma once

#include "Rockspackage.h"

struct Create 
{
    int handle_n {0};
    Slice key;
    Slice value;
};

struct Del_Read
{
    int handle_n {0};
    Slice key;
};

struct Read
{
    std::string value;
};

struct Update
{
    int handle_n {0};
    Slice key;
    Slice value;
};

struct FindPar 
{
    int handle_n {0};
};

struct FindRet
{
    std::vector<std::string> values;
};