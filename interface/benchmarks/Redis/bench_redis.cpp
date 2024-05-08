#include <benchmark/benchmark.h>

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include "../../RedisDB/include/RedisDBInterface.h"

using json = nlohmann::json;

const std::string path_dir = std::filesystem::current_path().remove_filename();

RedisDBInterface redis("tcp://127.0.0.1:6379");

// --benchmark_report_aggregates_only=true

//===----------------------------------------------------------------------===//
// CRUD methods benchmark.
//===----------------------------------------------------------------------===//

static void BM_Create(benchmark::State& state) {
  checkRedisConnection(redis.connection_);
  redis.connection_->flushdb();
  CreateRedis var("test_create_key", "nfType", "test_create_val");
  for (auto _ : state) {
    benchmark::DoNotOptimize(redis.create(var));
  }
  redis.connection_->flushdb();
}
BENCHMARK(BM_Create)->Iterations(1000)->Repetitions(5);

static void BM_Read(benchmark::State& state) {
  checkRedisConnection(redis.connection_);
  redis.connection_->flushdb();
  CreateRedis var("test_create_key", "nfType", "test_create_val");
  redis.create(var);
  for (auto _ : state) {
    benchmark::DoNotOptimize(
        redis.read(std::make_pair("test_create_key", "nfType")));
  }
  redis.connection_->flushdb();
}
BENCHMARK(BM_Read)->Iterations(1000)->Repetitions(5);

static void BM_Update(benchmark::State& state) {
  checkRedisConnection(redis.connection_);
  redis.connection_->flushdb();
  CreateRedis var("test_create_key", "nfType", "test_create_val");
  redis.create(var);
  for (auto _ : state) {
    benchmark::DoNotOptimize(
        redis.update(std::make_pair("test_create_key", "nfType"),
                     std::make_pair("new_val", "")));
  }
  redis.connection_->flushdb();
}
BENCHMARK(BM_Update)->Iterations(1000)->Repetitions(5);

static void BM_Del(benchmark::State& state) {
  checkRedisConnection(redis.connection_);
  redis.connection_->flushdb();
  CreateRedis var("test_create_key", "nfType", "test_create_val");
  redis.create(var);
  for (auto _ : state) {
    redis.del(std::make_pair("test_create_key", "nfType"));
  }
  redis.connection_->flushdb();
}
BENCHMARK(BM_Del)->Iterations(1)->Repetitions(100);

// ===----------------------------------------------------------------------===//
// benchmarks for general methods.
// ===----------------------------------------------------------------------===//

// ===**********************************************************************===//
// benchmark of find method.
// size:
// [50, 100, 250, 500]
// ===**********************************************************************===//
//*********************************************************===//

const std::vector<int64_t> values_find = {50, 100, 250, 500};

static void BM_find(benchmark::State& state) {
  checkRedisConnection(redis.connection_);
  redis.connection_->flushdb();
  int n = state.range(0);
  std::string path = path_dir + "../data_model.json";
  redis.createUniDB(path.c_str(), n);
  FindRedis findVar("NRF", n);
  for (auto _ : state) {
    benchmark::DoNotOptimize(redis.find(findVar));
  }
  redis.connection_->flushdb();
}
BENCHMARK(BM_find)->Iterations(5)->Repetitions(3)->Apply([](auto* b) {
  for (int64_t value : values_find) {
    b->Arg(value);
  }
});

//===**********************************************************************===//
/// benchmark of createUniDB method.
/// size:
/// [ 10'000, 20'000, 30'000 ]
//===**********************************************************************===//
/*
const std::vector<int64_t> values_create_db = { 10'000, 20'000, 30'000 };

static void BM_createUniDB(benchmark::State& state) {
    checkRedisConnection(redis.connection_);
    redis.connection_->flushdb();
    int n = state.range(0);
    for (auto _ : state) {
        redis.createUniDB("./data_model.json", n);
    }
    redis.connection_->flushdb();
}
BENCHMARK(BM_createUniDB)->Iterations(10)
                         ->Repetitions(5)
                         ->Apply([](auto* b) {
                               for (int64_t value : values_create_db) {
                                   b->Arg(value);
                                }
                           });
*/
//===**********************************************************************===//
// benchmarks for methods of combination.
//===**********************************************************************===//

static void BM_find_and_update_500(benchmark::State& state) {
  checkRedisConnection(redis.connection_);
  const size_t n = 500;

  redis.connection_->flushdb();
  std::string path = path_dir + "../data_model.json";
  redis.createUniDB(path.c_str(), n);
  std::ifstream cf(path.c_str());
  json instances = json::parse(cf);
  FindRedis findVarAMF("AMF", n);
  FindRedis findVarSMF("SMF", n);
  FindRedis findVarNRF("NRF", n);
  for (auto _ : state) {
    benchmark::DoNotOptimize(redis.find(findVarAMF));
    benchmark::DoNotOptimize(redis.find(findVarSMF));
    benchmark::DoNotOptimize(redis.find(findVarAMF));
    benchmark::DoNotOptimize(redis.find(findVarAMF));
    benchmark::DoNotOptimize(redis.find(findVarSMF));
    benchmark::DoNotOptimize(redis.update(
        std::make_pair(*(redis.find(findVarNRF).back()), "dataField"),
        std::make_pair(instances.dump(), "")));
    benchmark::DoNotOptimize(redis.find(findVarAMF));
    benchmark::DoNotOptimize(redis.find(findVarSMF));
    benchmark::DoNotOptimize(redis.find(findVarAMF));
    benchmark::DoNotOptimize(redis.find(findVarSMF));
    benchmark::DoNotOptimize(redis.find(findVarSMF));
  }
  cf.close();
  redis.connection_->flushdb();
}
BENCHMARK(BM_find_and_update_500)->Iterations(5)->Repetitions(5);

BENCHMARK_MAIN();
