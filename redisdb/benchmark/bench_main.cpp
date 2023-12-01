#include <benchmark/benchmark.h>

#include "../include/redis_find.h"

using json = nlohmann::json;

auto redis = std::make_unique<RedisInterface>("tcp://127.0.0.1:6379");

// --benchmark_report_aggregates_only=true

//===----------------------------------------------------------------------===//
// CRUD methods benchmark.
//===----------------------------------------------------------------------===//

static void BM_Create(benchmark::State& state) {
    redis->flushdb();
    for (auto _ : state) {
        redis->create("1", "1");
    }
    redis->flushdb();
}
BENCHMARK(BM_Create)->Iterations(100000)->Repetitions(5);

static void BM_HCreate(benchmark::State& state) {
    redis->flushdb();
    for (auto _ : state) {
        redis->hcreate("test_create_key", "nfType", "test_create_val");
    }
    redis->flushdb();
}
BENCHMARK(BM_HCreate)->Iterations(100000)->Repetitions(5);

static void BM_Read(benchmark::State& state) {
    redis->flushdb();
    redis->create("1", "1");
    for (auto _ : state) {
        redis->read("1");
    }
    redis->flushdb();
}
BENCHMARK(BM_Read)->Iterations(100000)->Repetitions(5);

static void BM_HRead(benchmark::State& state) {
    redis->flushdb();
    redis->hcreate("test_create_key", "nfType", "test_create_val");
    for (auto _ : state) {
        redis->hread("test_create_key", "nfType");
    }
    redis->flushdb();
}
BENCHMARK(BM_HRead)->Iterations(100000)->Repetitions(5);

static void BM_Update(benchmark::State& state) {
    redis->flushdb();
    redis->create("1", "1");
    for (auto _ : state) {
        redis->update("1", "1");
    }
    redis->flushdb();
}
BENCHMARK(BM_Update)->Iterations(100000)->Repetitions(5);

static void BM_HUpdate(benchmark::State& state) {
    redis->flushdb();
    for (auto _ : state) {
        redis->hupdate("test_create_key", "nfType", "test_create_val_new");
    }
    redis->flushdb();
}
BENCHMARK(BM_HUpdate)->Iterations(1)->Repetitions(100000);

static void BM_Del(benchmark::State& state) {
    redis->flushdb();
    redis->create("1", "1");
    for (auto _ : state) {
        redis->del("1");
    }
    redis->flushdb();
}
BENCHMARK(BM_Del)->Iterations(1)->Repetitions(100000);

static void BM_HDel(benchmark::State& state) {
    redis->flushdb();
    redis->hcreate("test_create_key", "nfType", "test_create_val");
    for (auto _ : state) {
        redis->hdel("test_create_key", "nfType");
        redis->hdel("test_create_key", "dataField");
    }
    redis->flushdb();
}
BENCHMARK(BM_HDel)->Iterations(1)->Repetitions(100000);

// ===----------------------------------------------------------------------===//
// benchmarks for general methods.
// ===----------------------------------------------------------------------===//

// ===**********************************************************************===//
// benchmark of find method.
// size:
// [10'000, 20'000, 40'000, 60'000, 80'000, 100'000]
// ===**********************************************************************===//

const std::vector<int64_t> values_find = { 10'000, 20'000, 40'000, 
                                           60'000, 80'000, 100'000 };

static void BM_find(benchmark::State& state) {
    redis->flushdb();
    int n = state.range(0);
    redis->createUniDB("../data_model.json", n);
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
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

const std::vector<int64_t> values_create_db = { 10'000, 20'000, 30'000 };

static void BM_createUniDB(benchmark::State& state) {
    redis->flushdb();
    int n = state.range(0);
    for (auto _ : state) {
        redis->createUniDB("../data_model.json", n);
    }
    redis->flushdb();
}
BENCHMARK(BM_createUniDB)->Iterations(10)
                         ->Repetitions(5)
                         ->Apply([](auto* b) {
                               for (int64_t value : values_create_db) {
                                   b->Arg(value);
                                }
                           });

//===**********************************************************************===//
// benchmarks for methods of combination.
//===**********************************************************************===//

static void BM_find_and_update_10000(benchmark::State& state) {
    redis->flushdb();
    redis->createUniDB("../data_model.json", 10000);

    std::ifstream cf("../data_model.json"); 
    json instances = json::parse(cf);  

    for (auto _ : state) {
        redis->hupdate(*(redis->find("NRF").back()), "dataField", instances.dump());
    }
    cf.close();
    redis->flushdb();
}
BENCHMARK(BM_find_and_update_10000)->Iterations(25)->Repetitions(25);

BENCHMARK_MAIN();