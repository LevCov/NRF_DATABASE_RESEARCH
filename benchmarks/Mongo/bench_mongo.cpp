#include <benchmark/benchmark.h>

#include "../../MongoDB/MongoDBInterface.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

MongoDBInterface mongo("mongodb://localhost:27017");
// --benchmark_report_aggregates_only=true

//===----------------------------------------------------------------------===//
// CRUD methods benchmark.
//===----------------------------------------------------------------------===//

static void BM_Create(benchmark::State& state) {
    mongo.set_db("NRF_DB");
    mongo.set_collection("NFs");
    auto doc_value = make_document(
        kvp("name", "MongoDB"));

    for (auto _ : state) {
        mongo.create(doc_value.view());
    }
    mongo.drop();
}
BENCHMARK(BM_Create)->Iterations(100000)->Repetitions(5);

static void BM_Read(benchmark::State& state) {
    mongo.set_db("NRF_DB");
    mongo.set_collection("NFs");
    auto doc_value = make_document(
        kvp("name", "MongoDB"));
    mongo.create(doc_value.view());
    for (auto _ : state) {
        mongo.read(make_document(
                kvp("name", "MongoDB")
                ));
    }
    mongo.drop();
}
BENCHMARK(BM_Read)->Iterations(100000)->Repetitions(5);

static void BM_Update(benchmark::State& state) {
    mongo.set_db("NRF_DB");
    mongo.set_collection("NFs");
    auto doc_value = make_document(
        kvp("name", "MongoDB"));
    mongo.create(doc_value.view());
    for (auto _ : state) {
        mongo.update(make_document(
                            kvp("name", "MongoDB")
                        ), 
                        make_document(
                            kvp("$set", make_document(
                                kvp("name", "mongodb")
                                ))
                            ));  
    }
    mongo.drop();
}
BENCHMARK(BM_Update)->Iterations(100000)->Repetitions(5);

static void BM_Del(benchmark::State& state) {
    mongo.set_db("NRF_DB");
    mongo.set_collection("NFs");
    auto doc_value = make_document(
        kvp("name", "MongoDB"));
    mongo.create(doc_value.view());
    for (auto _ : state) {
        mongo.del(make_document(
                    kvp("name", "MongoDB")
                    ));
    }
    mongo.drop();
}
BENCHMARK(BM_Del)->Iterations(1)->Repetitions(100);

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
    mongo.set_db("NRF_DB");
    mongo.set_collection("NFs");
    int n = state.range(0);
    mongo.createUniDB("/home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/benchmarks/data_model.json", n);
    
    for (auto _ : state) {
        mongo.find(make_document(
                kvp("nfType", "NRF")
            ));
    }
    mongo.drop();
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
    mongo.set_db("NRF_DB");
    mongo.set_collection("NFs");
    int n = state.range(0);
    for (auto _ : state) {
        mongo.createUniDB("/home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/benchmarks/data_model.json", n);
    }
    mongo.drop();
}
BENCHMARK(BM_createUniDB)->Iterations(3)
                         ->Repetitions(3)
                         ->Apply([](auto* b) {
                               for (int64_t value : values_create_db) {
                                   b->Arg(value);
                                }
                           });
*/
BENCHMARK_MAIN();