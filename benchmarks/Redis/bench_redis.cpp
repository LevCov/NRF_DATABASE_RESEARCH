#include <benchmark/benchmark.h>

#include "../../RedisDB/RedisDBInterface.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;
using StrView = sw::redis::StringView;

const std::string path_dir = std::filesystem::current_path().remove_filename();

RedisDBInterface redis("tcp://127.0.0.1:6379");

// --benchmark_report_aggregates_only=true

//===----------------------------------------------------------------------===//
// CRUD methods benchmark.
//===----------------------------------------------------------------------===//

static void BM_Create(benchmark::State& state) {
    redis.conn->flushdb();
    for (auto _ : state) {
        benchmark::DoNotOptimize(redis.create(std::pair<StrView,StrView>("1", "1")));
    }
    redis.conn->flushdb();
}
BENCHMARK(BM_Create)->Iterations(1000)->Repetitions(5);

static void BM_HCreate(benchmark::State& state) {
    redis.conn->flushdb();
    for (auto _ : state) {
        benchmark::DoNotOptimize(redis.hcreate("test_create_key", "nfType", "test_create_val"));
    }
    redis.conn->flushdb();
}
BENCHMARK(BM_HCreate)->Iterations(1000)->Repetitions(5);

static void BM_Read(benchmark::State& state) {
    redis.conn->flushdb();
    redis.create(std::pair<StrView,StrView>("1", "1"));
    for (auto _ : state) {
        benchmark::DoNotOptimize(redis.read("1"));
    }
    redis.conn->flushdb();
}
BENCHMARK(BM_Read)->Iterations(1000)->Repetitions(5);

static void BM_HRead(benchmark::State& state) {
    redis.conn->flushdb();
    redis.hcreate("test_create_key", "nfType", "test_create_val");
    for (auto _ : state) {
        benchmark::DoNotOptimize(redis.hread("test_create_key", "nfType"));
    }
    redis.conn->flushdb();
}
BENCHMARK(BM_HRead)->Iterations(1000)->Repetitions(5);

static void BM_Update(benchmark::State& state) {
    redis.conn->flushdb();
    redis.create(std::pair<StrView,StrView>("1", "1"));
    for (auto _ : state) {
        benchmark::DoNotOptimize(redis.update("1", "1"));
    }
    redis.conn->flushdb();
}
BENCHMARK(BM_Update)->Iterations(1000)->Repetitions(5);

static void BM_HUpdate(benchmark::State& state) {
    redis.conn->flushdb();
    for (auto _ : state) {
        benchmark::DoNotOptimize(redis.hupdate("test_create_key", "nfType", "test_create_val_new"));
    }
    redis.conn->flushdb();
}
BENCHMARK(BM_HUpdate)->Iterations(1)->Repetitions(100);

static void BM_Del(benchmark::State& state) {
    redis.conn->flushdb();
    redis.create(std::pair<StrView,StrView>("1", "1"));
    for (auto _ : state) {
        redis.del("1");
    }
    redis.conn->flushdb();
}
BENCHMARK(BM_Del)->Iterations(1)->Repetitions(100);

static void BM_HDel(benchmark::State& state) {
    redis.conn->flushdb();
    redis.hcreate("test_create_key", "nfType", "test_create_val");
    for (auto _ : state) {
        redis.hdel("test_create_key", "nfType");
        redis.hdel("test_create_key", "dataField");
    }
    redis.conn->flushdb();
}
BENCHMARK(BM_HDel)->Iterations(1)->Repetitions(100);

// ===----------------------------------------------------------------------===//
// benchmarks for general methods.
// ===----------------------------------------------------------------------===//

// ===**********************************************************************===//
// benchmark of find method.
// size:
// [50, 100, 250, 500]
// ===**********************************************************************===//
//*********************************************************===//

const std::vector<int64_t> values_find = { 50, 100, 250, 500};

static void BM_find(benchmark::State& state) {
    redis.conn->flushdb();
    int n = state.range(0);
    std::string path = path_dir + "../data_model.json";
    const char *cstr = path.c_str();
    redis.createUniDB(cstr, n);
    for (auto _ : state) {
        //redis.find("NRF");
        benchmark::DoNotOptimize(redis.find("NRF"));
    }
    redis.conn->flushdb();
}
BENCHMARK(BM_find)->Iterations(5)
                  ->Repetitions(3)
                  ->Apply([](auto* b) {
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
    redis.conn->flushdb();
    int n = state.range(0);
    for (auto _ : state) {
        redis.createUniDB("./data_model.json", n);
    }
    redis.conn->flushdb();
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
    redis.conn->flushdb();
    std::string path = path_dir + "../data_model.json";
    const char *cstr = path.c_str();
    redis.createUniDB(cstr, 500);
    std::ifstream cf(cstr);
    json instances = json::parse(cf);
    for (auto _ : state) {
        benchmark::DoNotOptimize(redis.find("AMF"));
        benchmark::DoNotOptimize(redis.find("SMF"));
        benchmark::DoNotOptimize(redis.find("AMF"));
        benchmark::DoNotOptimize(redis.find("AMF"));
        benchmark::DoNotOptimize(redis.find("SMF"));
        benchmark::DoNotOptimize(redis.hupdate(*(redis.find("NRF").back()), "dataField", instances.dump()));
        benchmark::DoNotOptimize(redis.find("AMF"));
        benchmark::DoNotOptimize(redis.find("SMF"));
        benchmark::DoNotOptimize(redis.find("AMF"));
        benchmark::DoNotOptimize(redis.find("SMF"));
        benchmark::DoNotOptimize(redis.find("SMF"));
    }
    cf.close();
    redis.conn->flushdb();
}
BENCHMARK(BM_find_and_update_500)->Iterations(5)->Repetitions(5);

BENCHMARK_MAIN();