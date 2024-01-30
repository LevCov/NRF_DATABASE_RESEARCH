cmake_minimum_required(VERSION 3.0)

project(RedisBenchmarkTest VERSION 0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(TEST_TARGET utest)

enable_testing()

add_subdirectory(googletest)

add_executable(utest
    ../src/redis_find.cpp
    test_code/redis_test.cpp
    test_main.cpp
)

target_include_directories(utest
    PUBLIC
        ${PROJECT_SOURCE_DIR}
    PRIVATE
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(GTest REQUIRED)
find_package(hiredis REQUIRED)
find_package(redis++ REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(uuid_v4 REQUIRED)

if(uuid_v4_FOUND)
    message(STATUS "Библиотека найдена")
else()
    message(STATUS "Библиотека не найдена")
endif()

find_path(UUID_V4_INCLUDE_DIRS uuid_v4)

if(UUID_V4_INCLUDE_DIRS)
  message("Найдена директория с uuid_v4.h: ${UUID_V4_INCLUDE_DIRS}")
else()
  message(FATAL_ERROR "Не удалось найти директорию с uuid_v4.h")
endif()

target_link_libraries(utest
    PRIVATE
        nlohmann_json::nlohmann_json
        hiredis::hiredis
        redis++::redis++
        GTest::gtest
        GTest::gtest_main
        GTest::gmock_main
        pthread
        uuid_v4::uuid_v4
)
include(GoogleTest)
gtest_discover_tests(utest)
