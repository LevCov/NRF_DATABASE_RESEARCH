#include <benchmark/benchmark.h>

#include <filesystem>

#include "../include/couchbase.h"
#include "bench_settings.h"

using CBI = CouchBaseInterface;

const std::string pathDir = std::filesystem::current_path().remove_filename();

static void getCallback(lcb_INSTANCE *, int, const lcb_RESPGET *resp) {
  Result_ *result = nullptr;
  lcb_respget_cookie(resp, reinterpret_cast<void **>(&result));

  result->status = lcb_respget_status(resp);
  result->value.clear();  // Remove any prior value
  if (result->status == LCB_SUCCESS) {
    const char *buf = nullptr;
    std::size_t buf_len = 0;
    CBI::check(lcb_respget_value(resp, &buf, &buf_len),
               "extract value from GET response");
    result->value.assign(buf, buf_len);
  }
}

static lcb_INSTANCE *init() {
  const std::string username{"Administrator"};
  const std::string password{"000000"};
  const std::string connectionString{"couchbase://127.0.0.1"};
  const std::string bucketName{"test_bucket"};

  lcb_CREATEOPTS *createOptions = nullptr;
  CBI::check(lcb_createopts_create(&createOptions, LCB_TYPE_BUCKET),
             "build options object for lcb_create");
  CBI::check(lcb_createopts_credentials(createOptions, username.c_str(),
                                        username.size(), password.c_str(),
                                        password.size()),
             "assign credentials");
  CBI::check(lcb_createopts_connstr(createOptions, connectionString.c_str(),
                                    connectionString.size()),
             "assign connection string");
  CBI::check(lcb_createopts_bucket(createOptions, bucketName.c_str(),
                                   bucketName.size()),
             "assign bucket name");

  lcb_INSTANCE *instance = nullptr;
  CBI::check(lcb_create(&instance, createOptions), "create lcb_INSTANCE");
  CBI::check(lcb_createopts_destroy(createOptions), "destroy options object");
  CBI::check(lcb_connect(instance), "schedule connection");
  CBI::check(lcb_wait(instance, LCB_WAIT_DEFAULT), "wait for connection");
  CBI::check(lcb_get_bootstrap_status(instance), "check bootstrap status");
  return instance;
}

CBI database;

// ./bench --benchmark_report_aggregates_only=true

//===----------------------------------------------------------------------===//
// CRUD methods benchmark.
//===----------------------------------------------------------------------===//

// static void BM_Create(benchmark::State &state) {
//   std::cout << pathDir;

//   auto instance = init();
//   lcb_install_callback(instance, LCB_CALLBACK_GET,
//                        reinterpret_cast<lcb_RESPCALLBACK>(getCallback));

//   const std::string key{"0"}, value{R"({"some":"json"})"};
//   for (auto _ : state) {
//     database.create(instance, key, value);
//   }
// }
// BENCHMARK(BM_Create)->Iterations(1000)->Repetitions(5);

// static void BM_Read(benchmark::State &state) {
//   auto instance = init();
//   lcb_install_callback(instance, LCB_CALLBACK_GET,
//                        reinterpret_cast<lcb_RESPCALLBACK>(getCallback));

//   const std::string key{"0"}, value{R"({"some":"json"})"};
//   database.create(instance, key, value);
//   for (auto _ : state) {
//     Result_ get_cbi;
//     database.search(instance, key, get_cbi);
//   }
// }
// BENCHMARK(BM_Read)->Iterations(1000)->Repetitions(5);

// static void BM_Update(benchmark::State &state) {
//   auto instance = init();
//   lcb_install_callback(instance, LCB_CALLBACK_GET,
//                        reinterpret_cast<lcb_RESPCALLBACK>(getCallback));

//   const std::string key{"0"}, value{R"({"nfInstanceId":"0"})"};
//   database.create(instance, "someKey", value);
//   for (auto _ : state) {
//     database.update(instance, "someKey", value);
//   }
// }
// BENCHMARK(BM_Update)->Iterations(1000)->Repetitions(5);

// static void BM_Del(benchmark::State &state) {
//   auto instance = init();
//   lcb_install_callback(instance, LCB_CALLBACK_GET,
//                        reinterpret_cast<lcb_RESPCALLBACK>(getCallback));

//   const std::string key{"0"}, value{R"({"nfInstanceId":"0"})"};
//   database.create(instance, key, value);
//   for (auto _ : state) {
//     database.del(instance, key);
//   }
// }
// BENCHMARK(BM_Del)->Iterations(1)->Repetitions(100);

static void BM_Search(benchmark::State &state) {
  auto instance = init();
  lcb_install_callback(instance, LCB_CALLBACK_GET,
                       reinterpret_cast<lcb_RESPCALLBACK>(getCallback));

  int n = state.range(0);
  std::string path = pathDir + "data/data_model.json";

  database.createUniDB(path.c_str(), n, instance);

  const std::string bucketName{"test_bucket"};
  for (auto _ : state) {
    Rows result{};
    database.read(instance, bucketName, std::to_string(n - 1), result);
  }
}
BENCHMARK(BM_Search)->Iterations(5)->Repetitions(3)->Apply([](auto *b) {
  for (int64_t value : values_db) {
    b->Arg(value);
  }
});

BENCHMARK_MAIN();
