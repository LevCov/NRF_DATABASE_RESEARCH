#include <benchmark/benchmark.h>

#include "../../RocksDB/RocksDBInterface.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

RocksDBInterface rocks("/tmp/rocksdb");

// --benchmark_report_aggregates_only=true

//===----------------------------------------------------------------------===//
// CRUD methods benchmark.
//===----------------------------------------------------------------------===//

static void BM_Create(benchmark::State& state) {
    rocks.flushdb();
    for (auto _ : state) {
        rocks.create(Create{
            0, "key", "value"
        });
    }
    rocks.flushdb();
}
BENCHMARK(BM_Create)->Iterations(100000)->Repetitions(5);

static void BM_Read(benchmark::State& state) {
    rocks.flushdb();
    rocks.create(Create{
            0, "key", "value"
        });
    for (auto _ : state) {
        rocks.read(Del_Read{
            0, "key"
        });
    }
    rocks.flushdb();
}
BENCHMARK(BM_Read)->Iterations(100000)->Repetitions(5);

static void BM_Update(benchmark::State& state) {
    rocks.flushdb();
    rocks.create(Create{
            0, "key", "value"
        });
    for (auto _ : state) {
        rocks.update(Update{
            0, "key", ""
        }, Update{
            0, "", "value1"
        });
    }
    rocks.flushdb();
}
BENCHMARK(BM_Update)->Iterations(100000)->Repetitions(5);

static void BM_Del(benchmark::State& state) {
    rocks.flushdb();
    rocks.create(Create{
            0, "key", "value"
        });
    for (auto _ : state) {
        rocks.del(Del_Read{
            0, "key"
        });
    }
    rocks.flushdb();
}
BENCHMARK(BM_Del)->Iterations(1)->Repetitions(100000);

// ===----------------------------------------------------------------------===//
// benchmarks for general methods.
// ===----------------------------------------------------------------------===//

// ===**********************************************************************===//
// benchmark of find method.
// size:
// [10'000, 20'000, 40'000, 60'000, 80'000, 100'000]
// ===**********************************************************************===//
//*********************************************************===//

const std::vector<int64_t> values_find = { 10'000, 20'000, 40'000, 
                                           60'000, 80'000, 100'000 };

static void BM_find(benchmark::State& state) {
    rocks.flushdb();
    int n = state.range(0);
    rocks.createUniDB("/home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/benchmarks/data_model.json", n);
    for (auto _ : state) {
        rocks.find(FindPar{rocks.NF["NRF"]});
    }
    rocks.flushdb();
}
BENCHMARK(BM_find)->Iterations(5)
                  ->Repetitions(3)
                  ->Apply([](auto* b) {
                        for (int64_t value : values_find) {
                            b->Arg(value);
                        }
                    });
//===**********************************************************************===//
// benchmarks for methods of combination.
//===**********************************************************************===//


static void BM_find_and_update_10000(benchmark::State& state) {
    rocks.flushdb();
    rocks.createUniDB("/home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/benchmarks/data_model.json", 10000);
    std::ifstream cf("/home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/benchmarks/data_model.json"); 

    json instance = json::parse(cf);
    for (auto _ : state) {
        auto data = rocks.find(FindPar{
            rocks.NF["NRF"]
        });
        rocks.update(Update{
                rocks.NF["NRF"], data.values[0], ""},
            Update{
                rocks.NF["NRF"], "", instance.dump()});
    }
    cf.close();
    rocks.flushdb();
}
BENCHMARK(BM_find_and_update_10000)->Iterations(25)->Repetitions(25);

BENCHMARK_MAIN();