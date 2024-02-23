cmake_minimum_required(VERSION 3.22)

project(NRF VERSION 1.0)

add_executable(main main.cpp)

target_include_directories(main PUBLIC MongoDB RedisDB)

add_subdirectory(MongoDB)
add_subdirectory(RedisDB)

target_link_libraries(main PRIVATE Mongo Redis)