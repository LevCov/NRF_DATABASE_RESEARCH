cmake_minimum_required(VERSION 3.22)

project(Rocks VERSION 1.0)

add_library(Rocks STATIC RocksDBInterface.cpp)

target_include_directories(${PROJECT_NAME} 
PUBLIC
    ../include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#find_library(ROCKS_LIBRARY rocksdb)
target_link_libraries(${PROJECT_NAME} ${ROCKSDB_LIBRARY} rocksdb dl)

find_package(nlohmann_json REQUIRED)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)