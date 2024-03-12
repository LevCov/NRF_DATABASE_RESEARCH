cmake_minimum_required(VERSION 3.22)

project(RocksTest VERSION 0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(TEST_TARGET utest)


add_executable(utest
    test_rocks.cpp
    test_main.cpp
)

target_include_directories(utest PUBLIC ../../RocksDB)

add_subdirectory(../../RocksDB build/RocksDB)


target_include_directories(utest
    PUBLIC
        ${PROJECT_SOURCE_DIR}
    PRIVATE
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)


find_package(GTest REQUIRED)

enable_testing()

target_link_libraries(utest
    PRIVATE
        gtest
        gtest_main
        gmock
        gtest_main
        pthread
        Rocks
)
include(GoogleTest)
gtest_discover_tests(utest)
