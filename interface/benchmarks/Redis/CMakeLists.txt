cmake_minimum_required(VERSION 3.0)

project(RedisBenchmarkTest VERSION 0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(BENCH_TARGET bench)

add_executable(bench ../../RedisDB/src/redis_conn.cpp
                     ../../RedisDB/src/RedisDBInterface.cpp bench_redis.cpp)

target_include_directories(
  bench
  PUBLIC ${PROJECT_SOURCE_DIR}
  PRIVATE ${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}
          ${CMAKE_CURRENT_SOURCE_DIR})

find_package(hiredis REQUIRED)
find_package(redis++ REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(benchmark REQUIRED)

target_link_libraries(
  bench PRIVATE nlohmann_json::nlohmann_json hiredis::hiredis redis++::redis++
                benchmark::benchmark pthread)
