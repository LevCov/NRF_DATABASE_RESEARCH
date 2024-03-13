cmake_minimum_required(VERSION 3.1)

project(couchbase_benchmark VERSION 0.0)

set(SOURCES src/couchbase.cpp src/main.cpp)
set(CMAKE_PREFIX_PATH /opt/homebrew/Cellar/libcouchbase/3.3.12
                      ${CMAKE_PREFIX_PATH})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} ${SOURCES})

list(APPEND CMAKE_SYSTEM_PREFIX_PATH /opt/homebrew/Cellar/libcouchbase/3.3.12)

message("CMAKE_SYSTEM_PREFIX_PATH: ${CMAKE_SYSTEM_PREFIX_PATH}")
find_library(LIBCOUCHBASE_LIBRARY libcouchbase)
message("Libcouchbase: ${LIBCOUCHBASE_LIBRARY}")

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/include
  PRIVATE ${PROJECT_BINARY_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}
          ${CMAKE_CURRENT_SOURCE_DIR})

find_package(nlohmann_json REQUIRED)

set(Libcouchbase_DIR /opt/homebrew/Cellar/libcouchbase/3.3.12/lib/)
find_package(Libcouchbase REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE Libcouchbase::Libcouchbase)
