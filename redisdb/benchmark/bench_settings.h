#pragma once

#include <string>
#include <vector>

// tunes for CRUD methods.
const std::vector<int64_t> values_db = {10, 100, 1'000};

// tunes for find method.
const std::vector<int64_t> values_find = {10'000, 20'000, 40'000,
                                          60'000, 80'000, 100'000};

// tunes for create database method.
const std::vector<int64_t> values_create_db = {10'000, 15'000, 20'000};
