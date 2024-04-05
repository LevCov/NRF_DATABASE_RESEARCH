#include <benchmark/benchmark.h>

#include "../include/nfTypes.h"
#include "../include/redis_find.h"
#include "bench_settings.h"

using json = nlohmann::json;

auto redis = std::make_unique<RedisInterface>("tcp://127.0.0.1:6379");

// ./bench --benchmark_report_aggregates_only=true

//===----------------------------------------------------------------------===//
// CRUD methods benchmark.
//===----------------------------------------------------------------------===//

static void BM_Create(benchmark::State& state) {
  redis->flushdb();
  int n = state.range(0);
  redis->createUniDB("data_model.json", n);
  for (auto _ : state) {
    redis->create("test_create_key", "nfType", "test_create_val");
  }
  redis->flushdb();
}
BENCHMARK(BM_Create)->Iterations(100)->Repetitions(5)->Apply([](auto* b) {
  for (int64_t value : values_db) {
    b->Arg(value);
  }
});

static void BM_Read(benchmark::State& state) {
  redis->flushdb();
  int n = state.range(0);
  redis->createUniDB("data_model.json", n);
  redis->create("test_create_key", "nfType", "test_create_val");
  for (auto _ : state) {
    redis->hread("test_create_key", "nfType");
  }
  redis->flushdb();
}
BENCHMARK(BM_Read)->Iterations(100)->Repetitions(5)->Apply([](auto* b) {
  for (int64_t value : values_db) {
    b->Arg(value);
  }
});

// static void BM_Update(benchmark::State& state) {
//     redis->flushdb();
//     int n = state.range(0);benchmark/bench_main.cpp
//     redis->createUniDB("data_model.json", n);
//     for (auto _ : state) {
//         redis->update("test_create_key", "nfType", "test_create_val_new");
//     }
//     redis->flushdb();
// }
// BENCHMARK(BM_Update)->Iterations(1)
//                     ->Repetitions(10)
//                     ->Apply([](auto* b) {
//                            for (int64_t value : values_db) {
//                                b->Arg(value);
//                            }
//                       });

// static void BM_Del(benchmark::State& state) {
//     redis->flushdb();
//     int n = state.range(0);
//     redis->createUniDB("data_model.json", n);
//     redis->create("test_create_key", "nfType", "test_create_val");
//     for (auto _ : state) {
//         redis->del("test_create_key", "nfType");
//         redis->del("test_create_key", "dataField");
//     }
//     redis->flushdb();
// }
// BENCHMARK(BM_Del)->Iterations(1)
//                  ->Repetitions(10)
//                  ->Apply([](auto* b) {
//                        for (int64_t value : values_db) {
//                            b->Arg(value);
//                        }
//                    });

// ===----------------------------------------------------------------------===//
// benchmarks for general methods.
// ===----------------------------------------------------------------------===//

// ===**********************************************************************===//
// benchmark of find method.
// size in size in bench_settings.h
// ===**********************************************************************===//

// static void BM_find(benchmark::State& state) {
//     redis->flushdb();
//     int n = state.range(0);
//     redis->createUniDB("data_model.json", n);
//     for (auto _ : state) {
//         redis->find("NRF");
//     }
//     redis->flushdb();
// }
// BENCHMARK(BM_find)->Iterations(5)
//                   ->Repetitions(3)
//                   ->Apply([](auto* b) {
//                         for (int64_t value : values_find) {
//                             b->Arg(value);
//                         }
//                     });

//===**********************************************************************===//
/// benchmark of createUniDB method.
/// size in bench_settings.h
//===**********************************************************************===//

// static void BM_createUniDB(benchmark::State& state) {
//     redis->flushdb();
//     int n = state.range(0);
//     for (auto _ : state) {
//         redis->createUniDB("data_model.json", n);
//     }
//     redis->flushdb();
// }
// BENCHMARK(BM_createUniDB)->Iterations(10)
//                          ->Repetitions(5)
//                          ->Apply([](auto* b) {
//                                for (int64_t value : values_create_db) {
//                                    b->Arg(value);
//                                 }
//                            });

//===**********************************************************************===//
// benchmarks for methods of combination.
//===**********************************************************************===//

// static void BM_find_and_update_10000(benchmark::State& state) {
//     redis->flushdb();
//     redis->createUniDB("data_model.json", 10);

//     std::ifstream cf("data_model.json");
//     json instances = json::parse(cf);

//     for (auto _ : state) {
//         redis->update(*(redis->find("NRF").back()), "dataField",
//         instances.dump());
//     }
//     cf.close();
//     redis->flushdb();
// }
// BENCHMARK(BM_find_and_update_10000)->Iterations(5)->Repetitions(5);

BENCHMARK_MAIN();
