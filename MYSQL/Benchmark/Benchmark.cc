

#include "benchmark/benchmark.h"
#include <random>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <rapidjson/document.h>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
using namespace std;
using namespace rapidjson;

#include "UUIDv4.h"
#include "MYSQLConnector.h"






std::string tcp = "tcp://localhost:3306";
std::string root = "root";
std::string password = "0000";
std::string DBName = "test";
std::string path = "data_model.json";




 


 static void BM_Create(benchmark::State& state) {
    MYSQLInterface a(tcp, root, password, DBName);
    int i = 0;
    for (auto _ : state) {

     a.createTable(path, "nrf_10");
    i = i + 1;
     std::cout << i << endl;

   }
  
}
 BENCHMARK(BM_Create)->Iterations(1)->Repetitions(5);
 

 
 static void BM_Read(benchmark::State& state) {

   MYSQLInterface a(tcp, root, password, DBName);
  a.createTable(path, "NRF_10");

   a.update("nrf_10", "nfInstanceId","23232");
   for (auto _ : state) {
     a.read("nrf_10", "nfInstanceId");
   }
   
 }
 //BENCHMARK(BM_Read)->Iterations(1000)->Repetitions(5);


 
static void BM_Update(benchmark::State& state) {


  MYSQLInterface a(tcp, root, password, DBName);
  a.update("nrf_10", "nfInstanceId", "23232");
  int i = 0;
  for (auto _ : state) {
    
    a.update("nrf_10", "nfInstanceId", "to_string(i)");
    //i += 1;
  }
  
}
BENCHMARK(BM_Update)->Iterations(1000)->Repetitions(5);




 static void BM_Del(benchmark::State& state) {
  MYSQLInterface a(tcp, root, password, DBName);

  for (auto _ : state) {
    a.createTable(path, "nrf_10");
    a.del("nrf_10");
  }
  
}
BENCHMARK(BM_Del)->Iterations(1)->Repetitions(100);
 


const std::vector<int64_t> values_find = {50, 100, 250, 500};

static void BM_find(benchmark::State& state) {
  MYSQLInterface a(tcp, root, password, DBName);
  a.createTable(path, "nrf_10");



  for (auto _ : state) {

    a.read("nrf_10", "nfStatus");
  }
  
}
BENCHMARK(BM_find)->Iterations(5)->Repetitions(3)->Apply([](auto* b) {
  for (int64_t value : values_find) {
    b->Arg(value);
  }


});




static void BM_find_and_update_500(benchmark::State& state) {
  MYSQLInterface a(tcp, root, password, DBName);

  a.createUniDB(path, 500);

  for (auto _ : state) {
    benchmark::DoNotOptimize(a.find("nfType", "AMF"));
    std::cout << "1" << std::endl;
    benchmark::DoNotOptimize(a.find("nfType", "SMF"));
    std::cout << "2" << std::endl;
    benchmark::DoNotOptimize(a.find("nfType", "AMF"));
    std::cout << "3" << std::endl;    benchmark::DoNotOptimize(a.find("nfType", "AMF"));
    std::cout << "4" << std::endl;
    benchmark::DoNotOptimize(a.find("nfType", "SMF"));
    std::cout << "5" << std::endl;
    
    a.update("nrf_10", "nfType", " ");

    benchmark::DoNotOptimize(a.find("nfType", "AMF"));
    std::cout << "6" << std::endl;
    benchmark::DoNotOptimize(a.find("nfType", "SMF"));
    std::cout << "7" << std::endl;
    benchmark::DoNotOptimize(a.find("nfType", "AMF"));
    std::cout << "8" << std::endl;
    benchmark::DoNotOptimize(a.find("nfType", "SMF"));
    std::cout << "9" << std::endl;
    benchmark::DoNotOptimize(a.find("nfType", "SMF"));
    std::cout << "10" << std::endl;

  }
  a.dropUniDB(500);


}
BENCHMARK(BM_find_and_update_500)->Iterations(5)->Repetitions(5);




BENCHMARK_MAIN();

