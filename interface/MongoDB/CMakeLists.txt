cmake_minimum_required(VERSION 3.22)

project(Mongo VERSION 1.0)

add_library(Mongo STATIC mongod_conn.cpp MongoDBInterface.cpp)

target_include_directories(${PROJECT_NAME} 
PUBLIC
    ../include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)

include_directories(${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} mongo::bsoncxx_shared)
target_link_libraries(${PROJECT_NAME} mongo::mongocxx_shared)