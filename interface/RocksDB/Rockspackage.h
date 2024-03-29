#pragma once

// взаимодейсвтие с БД
#include "rocksdb/db.h"
#include "rocksdb/options.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
// взаимодействие с json file
#include <nlohmann/json.hpp>

using ROCKSDB_NAMESPACE::ReadOptions;
using ROCKSDB_NAMESPACE::WriteOptions;
using ROCKSDB_NAMESPACE::ColumnFamilyHandle;
using ROCKSDB_NAMESPACE::ColumnFamilyDescriptor;
using ROCKSDB_NAMESPACE::ColumnFamilyOptions;
using ROCKSDB_NAMESPACE::Slice;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::Options;
using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::DBOptions;

using json = nlohmann::json;