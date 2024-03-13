cmake_minimum_required(VERSION 3.22)

project(Redis VERSION 1.0)

add_library(Redis STATIC redis_conn.cpp RedisDBInterface.cpp)

target_include_directories(${PROJECT_NAME} 
PUBLIC
    ../include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_path(HIREDIS_HEADER hiredis)
target_include_directories(${PROJECT_NAME} PUBLIC ${HIREDIS_HEADER})

find_library(HIREDIS_LIB hiredis)
target_link_libraries(${PROJECT_NAME} ${HIREDIS_LIB})

find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories(${PROJECT_NAME} PUBLIC ${REDIS_PLUS_PLUS_HEADER})

find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries(${PROJECT_NAME} ${REDIS_PLUS_PLUS_LIB})

find_package(nlohmann_json REQUIRED)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)

# find_package(hiredis REQUIRED)
# find_package(redis++ REQUIRED)

# target_link_libraries(${PROJECT_NAME} PRIVATE hiredis::hiredis_static)
# target_link_libraries(${PROJECT_NAME} PRIVATE redis++::redis++_static)