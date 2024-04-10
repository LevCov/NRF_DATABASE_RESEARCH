cmake_minimum_required(VERSION 3.0)

project(RedisBenchmarkTest VERSION 0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(TEST_TARGET utest)

enable_testing()

add_executable(
  utest ../../RedisDB/src/redis_conn.cpp ../../RedisDB/src/RedisDBInterface.cpp
        test_redis.cpp test_main.cpp)

target_include_directories(
  utest
  PUBLIC ${PROJECT_SOURCE_DIR}
  PRIVATE ${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}
          ${CMAKE_CURRENT_SOURCE_DIR})

find_package(GTest REQUIRED)
find_package(hiredis REQUIRED)
find_package(redis++ REQUIRED)
find_package(nlohmann_json REQUIRED)

target_link_libraries(
  utest
  PRIVATE nlohmann_json::nlohmann_json
          hiredis::hiredis
          redis++::redis++
          GTest::gtest
          GTest::gtest_main
          GTest::gmock_main
          pthread)
include(GoogleTest)
gtest_discover_tests(utest)
