#include <benchmark/benchmark.h>

#include "../../RocksDB/RocksDBInterface.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

const std::string path_dir = std::filesystem::current_path().remove_filename();

RocksDBInterface rocks("/tmp/rocksdb");

// --benchmark_report_aggregates_only=true

//===----------------------------------------------------------------------===//
// CRUD methods benchmark.
//===----------------------------------------------------------------------===//

static void BM_Create(benchmark::State& state) {
    rocks.flushdb();
    for (auto _ : state) {
        benchmark::DoNotOptimize(rocks.create(Create{
            0, "key", "value"
        }));
    }
    rocks.flushdb();
}
BENCHMARK(BM_Create)->Iterations(1000)->Repetitions(5);

static void BM_Read(benchmark::State& state) {
    rocks.flushdb();
    rocks.create(Create{
            0, "key", "value"
        });
    for (auto _ : state) {
        benchmark::DoNotOptimize(rocks.read(Del_Read{
            0, "key"
        }));
    }
    rocks.flushdb();
}
BENCHMARK(BM_Read)->Iterations(1000)->Repetitions(5);

static void BM_Update(benchmark::State& state) {
    rocks.flushdb();
    rocks.create(Create{
            0, "key", "value"
        });
    for (auto _ : state) {
        benchmark::DoNotOptimize(rocks.update(Update{
            0, "key", ""
        }, Update{
            0, "", "value1"
        }));
    }
    rocks.flushdb();
}
BENCHMARK(BM_Update)->Iterations(1000)->Repetitions(5);

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

const std::vector<int64_t> values_find = { 50, 100, 250, 500};

static void BM_find(benchmark::State& state) {
    rocks.flushdb();
    int n = state.range(0);
    std::string path = path_dir + "../data_model.json";
    const char *cstr = path.c_str();
    rocks.createUniDB(cstr, n);
    for (auto _ : state) {
        //rocks.find(FindPar{rocks.NF["NRF"]});
        benchmark::DoNotOptimize(rocks.find(FindPar{rocks.NF["NRF"]}));
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


static void BM_find_and_update_500(benchmark::State& state) {
    rocks.flushdb();
    std::string path = path_dir + "../data_model.json";
    const char *cstr = path.c_str();
    rocks.createUniDB(cstr, 500);
    std::ifstream cf(cstr); 

    json instance = json::parse(cf);
    for (auto _ : state) {
        benchmark::DoNotOptimize(rocks.find(FindPar{rocks.NF["SMF"]}));
        benchmark::DoNotOptimize(rocks.find(FindPar{rocks.NF["AMF"]}));
        benchmark::DoNotOptimize(rocks.find(FindPar{rocks.NF["SMF"]}));
        benchmark::DoNotOptimize(rocks.find(FindPar{rocks.NF["SMF"]}));
        benchmark::DoNotOptimize(rocks.find(FindPar{rocks.NF["AMF"]}));
        auto data = rocks.find(FindPar{
            rocks.NF["NRF"]
        });
        rocks.update(Update{
                rocks.NF["NRF"], data.values[0], ""},
            Update{
                rocks.NF["NRF"], "", instance.dump()});
        benchmark::DoNotOptimize(rocks.find(FindPar{rocks.NF["AMF"]}));
        benchmark::DoNotOptimize(rocks.find(FindPar{rocks.NF["SMF"]}));
        benchmark::DoNotOptimize(rocks.find(FindPar{rocks.NF["AMF"]}));
        benchmark::DoNotOptimize(rocks.find(FindPar{rocks.NF["AMF"]}));
        benchmark::DoNotOptimize(rocks.find(FindPar{rocks.NF["SMF"]}));
    }
    cf.close();
    rocks.flushdb();
}
BENCHMARK(BM_find_and_update_500)->Iterations(5)->Repetitions(5);

BENCHMARK_MAIN();