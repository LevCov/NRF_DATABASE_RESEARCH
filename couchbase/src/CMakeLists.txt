cmake_minimum_required(VERSION 3.22)

project(RedisBenchmarkTest VERSION 0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(BENCH_TARGET bench)

add_executable(bench couchbase.cpp couchbaseBench.cpp)

target_include_directories(
  bench
  PUBLIC ${PROJECT_SOURCE_DIR}
  PRIVATE ${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}
          ${CMAKE_CURRENT_SOURCE_DIR})

find_package(benchmark REQUIRED)

find_package(couchbase REQUIRED)
if(NOT couchbase_FOUND)
  message(SEND_ERROR "Failed to find SDL")
  return()
else()
  include_directories(${couchbase_INCLUDE_DIR})
endif()

target_link_libraries(bench PRIVATE couchbase benchmark::benchmark pthread)
