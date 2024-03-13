cmake_minimum_required(VERSION 3.0)

project(RedisBenchmarkTest VERSION 0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(BENCH_TARGET bench)

add_executable(bench
    bench_redis.cpp
)

target_include_directories(bench PUBLIC ../../RedisDB)

add_subdirectory(../../RedisDB build/RedisDB)

target_include_directories(bench
    PUBLIC
        ${PROJECT_SOURCE_DIR}
    PRIVATE
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(benchmark REQUIRED)

target_link_libraries(bench
    PRIVATE
        benchmark
        pthread
        Redis
)
