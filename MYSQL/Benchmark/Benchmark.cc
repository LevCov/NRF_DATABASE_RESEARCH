

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




 

//Бенчмарк созадния таблиц с данными.
//Всего 5 проходов, в каждом проходе созадется 1000 таблиц.
 static void BM_Create(benchmark::State& state) {
    MYSQLInterface a(tcp, root, password, DBName);
    int i = 0;
    for (auto _ : state) {

     a.createTable(path, "nrf_10");
    i = i + 1;
     std::cout << i << endl;

   }
  
}
 BENCHMARK(BM_Create)->Iterations(1000)->Repetitions(5);
 

 //Бенчмарк чтения из таблицы.
 //Всего 5 проходов, в каждом проходе информация считывается 1000 раз.
 static void BM_Read(benchmark::State& state) {

   MYSQLInterface a(tcp, root, password, DBName);
  a.createTable(path, "NRF_10");

   a.update("nrf_10", "nfInstanceId","23232");
   for (auto _ : state) {
     a.read("nrf_10", "nfInstanceId");
   }
   
 }
 BENCHMARK(BM_Read)->Iterations(1000)->Repetitions(5);


//Бенчмарк обновления данных в таблице.
//Всего 5 проходов, в каждом проходе поле обновляется 1000 раз.
static void BM_Update(benchmark::State& state) {


  MYSQLInterface a(tcp, root, password, DBName);
  a.update("nrf_10", "nfInstanceId", "23232");
  int i = 0;
  for (auto _ : state) {
    
    a.update("nrf_10", "nfInstanceId", "to_string(i)");
    i += 1;
  }
  
}
BENCHMARK(BM_Update)->Iterations(1000)->Repetitions(5);



//Бенчмарк удаления таблицы.
//1 удаления за каждый проход. Всего 1000 проходов.
 static void BM_Del(benchmark::State& state) {
  MYSQLInterface a(tcp, root, password, DBName);

  for (auto _ : state) {
    a.createTable(path, "nrf_10");
    a.del("nrf_10");
  }
  
}
BENCHMARK(BM_Del)->Iterations(1)->Repetitions(100);
 


const std::vector<int64_t> values_find = {50, 100, 250, 500;


//Бенчмарк поиска данных в базе данных.
//Всего 3 проходов, в каждом проходе пооиск происходит  5, 50, 100, 250, 500 раз. 
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



//Основной бенчмарк работы сетевой функции NRF. Происходит измерение скорости срабатывания методов в сценариии поиск + обновление.
//Всего 5 проходов, в каждом проходе сценарий поиск + обновление срабатывает 5 раз.
static void BM_find_and_update_500(benchmark::State& state) {
  MYSQLInterface a(tcp, root, password, DBName);

  a.createUniDB(path, 500);

  for (auto _ : state) {
    benchmark::DoNotOptimize(a.find("nfType", "AMF"));
    benchmark::DoNotOptimize(a.find("nfType", "SMF"));
    benchmark::DoNotOptimize(a.find("nfType", "AMF"));
    benchmark::DoNotOptimize(a.find("nfType", "AMF"));
    benchmark::DoNotOptimize(a.find("nfType", "SMF"));
    
    a.update("nrf_10", "nfType", " ");

    benchmark::DoNotOptimize(a.find("nfType", "AMF"));
    benchmark::DoNotOptimize(a.find("nfType", "SMF"));
    benchmark::DoNotOptimize(a.find("nfType", "AMF"));
    benchmark::DoNotOptimize(a.find("nfType", "SMF"));
    benchmark::DoNotOptimize(a.find("nfType", "SMF"));
   

  }
  a.dropUniDB(500);


}
BENCHMARK(BM_find_and_update_500)->Iterations(5)->Repetitions(5);




BENCHMARK_MAIN();

