#include <benchmark/benchmark.h>

#include <filesystem>

#include "../../couchbase/include/couchbase.h"
#include "bench_settings.h"

const std::string pathDir = std::filesystem::current_path().remove_filename();

auto database = std::make_unique<CouchBaseInterface>();
// ./bench --benchmark_report_aggregates_only=true

namespace {
using key_tt = std::string_view;
using value_t = std::string_view;
}  // namespace

const std::pair<key_tt, value_t> keyVal({"0", R"({"some":"json"})"});

//===----------------------------------------------------------------------===//
// CRUD methods benchmark.
//===----------------------------------------------------------------------===//

static void BM_Search(benchmark::State &state) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  const std::string_view bucketName{"test_bucket"};
  database->flushdb();
  database->createUniDB(
      "/Users/georgryabov/Desktop/main/wtf/NRF_DATABASE_RESEARCH/interface/"
      "couchbase/data/test.json",
      10);
  for (auto _ : state) {
    database->find(std::make_pair(bucketName, "CHF"));
  }
}
BENCHMARK(BM_Search)->Iterations(1)->Repetitions(1)->Apply([](auto *b) {
  for (int64_t value : values_db) {
    b->Arg(value);
  }
});

static void BM_Create(benchmark::State &state) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  for (auto _ : state) {
    database->create(keyVal);
  }
}
BENCHMARK(BM_Create)->Iterations(1000)->Repetitions(5);

static void BM_Read(benchmark::State &state) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  database->create(keyVal);
  for (auto _ : state) {
    database->read(keyVal.first);
    database->result_ = {};
  }
}
BENCHMARK(BM_Read)->Iterations(1000)->Repetitions(5);

static void BM_Update(benchmark::State &state) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  database->create(keyVal);
  for (auto _ : state) {
    database->update("someKey", keyVal.second);
  }
}
BENCHMARK(BM_Update)->Iterations(1000)->Repetitions(5);

static void BM_Del(benchmark::State &state) {
  lcb_install_callback(database->instance_, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));
  database->flushdb();
  database->create(keyVal);
  for (auto _ : state) {
    database->del(keyVal.first);
  }
  database->flushdb();
}
BENCHMARK(BM_Del)->Iterations(1)->Repetitions(10);

BENCHMARK_MAIN();
