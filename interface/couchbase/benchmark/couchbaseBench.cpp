#include <benchmark/benchmark.h>

#include <filesystem>

#include "../include/couchbase.h"
#include "bench_settings.h"

const std::string pathDir = std::filesystem::current_path().remove_filename();

auto database = std::make_unique<CouchBaseInterface>();
// ./bench --benchmark_report_aggregates_only=true

//===----------------------------------------------------------------------===//
// CRUD methods benchmark.
//===----------------------------------------------------------------------===//

static void BM_Create(benchmark::State &state) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  const std::string key{"0"}, value{R"({"some":"json"})"};
  for (auto _ : state) {
    database->create(std::make_pair(key, value));
  }
}
BENCHMARK(BM_Create)->Iterations(1000)->Repetitions(5);

static void BM_Read(benchmark::State &state) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  const std::string key{"0"}, value{R"({"some":"json"})"};
  database->create(std::make_pair(key, value));
  for (auto _ : state) {
    database->read(key);
    database->result_ = {};
  }
}
BENCHMARK(BM_Read)->Iterations(1000)->Repetitions(5);

static void BM_Update(benchmark::State &state) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  const std::string key{"0"}, value{R"({"nfInstanceId":"0"})"};
  database->create(std::make_pair(key, value));
  for (auto _ : state) {
    database->update("someKey", value);
  }
}
BENCHMARK(BM_Update)->Iterations(1000)->Repetitions(5);

static void BM_Del(benchmark::State &state) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  const std::string key{"0"}, value{R"({"nfInstanceId":"0"})"};
  database->create(std::make_pair(key, value));
  for (auto _ : state) {
    database->del(key);
  }
}
BENCHMARK(BM_Del)->Iterations(1)->Repetitions(100);

static void BM_Search(benchmark::State &state) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  std::string path = pathDir + "benchmarks/data_model.json";
  const std::string bucketName{"test_bucket"};
  const int n = state.range(0);
  database->createUniDB(path.c_str(), n);
  for (auto _ : state) {
    database->find(std::make_pair(bucketName, "PCF"));
    database->resultRows_ = {};
  }
}
BENCHMARK(BM_Search)->Iterations(1)->Repetitions(1)->Apply([](auto *b) {
  for (int64_t value : values_db) {
    b->Arg(value);
  }
});

BENCHMARK_MAIN();
