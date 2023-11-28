#include <benchmark/benchmark.h>

#include "../include/redis_find.h"

using json = nlohmann::json;

auto redis = std::make_unique<RedisInterface>("tcp://127.0.0.1:6379");

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

//===----------------------------------------------------------------------===//
// benchmarks for general methods.
//===----------------------------------------------------------------------===//

//===**********************************************************************===//
// group benchmarks of find method.
// size:
// [1, 10, 100, 1000, 10'000, 20'000, 40'000, 60'000, 80'000, 100'000, 400'000]
//===**********************************************************************===//

static void BM_find_1(benchmark::State& state) {
    redis->flushdb();
    redis->createUniDB("../data_model.json", 1);
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_1)->Iterations(1000)->Repetitions(20);

static void BM_find_10(benchmark::State& state) {
    redis->flushdb();
    redis->createUniDB("../data_model.json", 10);
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_10)->Iterations(1000)->Repetitions(20);

static void BM_find_100(benchmark::State& state) {
    redis->flushdb();
    redis->createUniDB("../data_model.json", 100);
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_100)->Iterations(1000)->Repetitions(20);

static void BM_find_1000(benchmark::State& state) {
    redis->flushdb();
    redis->createUniDB("../data_model.json", 1000);
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_1000)->Iterations(100)->Repetitions(5);

static void BM_find_10000(benchmark::State& state) {
    redis->flushdb();
    redis->createUniDB("../data_model.json", 10000);
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_10000)->Iterations(25)->Repetitions(5);

static void BM_find_20000(benchmark::State& state) {
    redis->flushdb();
    redis->createUniDB("../data_model.json", 20000);
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_20000)->Iterations(25)->Repetitions(5);

static void BM_find_40000(benchmark::State& state) {
    redis->flushdb();
    redis->createUniDB("../data_model.json", 40000);
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_40000)->Iterations(25)->Repetitions(5);

static void BM_find_60000(benchmark::State& state) {
    redis->flushdb();
    redis->createUniDB("../data_model.json", 60000);
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_60000)->Iterations(10)->Repetitions(5);

static void BM_find_80000(benchmark::State& state) {
    redis->flushdb();
    redis->createUniDB("../data_model.json", 80000);
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_80000)->Iterations(10)->Repetitions(5);

static void BM_find_100000(benchmark::State& state) {
    redis->flushdb();
    redis->createUniDB("../data_model.json", 100000);
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_100000)->Iterations(10)->Repetitions(5);

//===**********************************************************************===//
/// group benchmarks of find method.
/// size:
/// [1, 1000, 10'000, 100'000]
//===**********************************************************************===//

static void BM_createUniDB_1(benchmark::State& state) {
    redis->flushdb();
    for (auto _ : state) {
        redis->createUniDB("../data_model.json", 1);
    }
    redis->flushdb();
}
BENCHMARK(BM_createUniDB_1)->Iterations(100)->Repetitions(10);

static void BM_createUniDB_1000(benchmark::State& state) {
    redis->flushdb();
    for (auto _ : state) {
        redis->createUniDB("../data_model.json", 1000);
    }
    redis->flushdb();
}
BENCHMARK(BM_createUniDB_1000)->Iterations(100)->Repetitions(10);

static void BM_createUniDB_10000(benchmark::State& state) {
    redis->flushdb();
    for (auto _ : state) {
        redis->createUniDB("../data_model.json", 10000);
    }
    redis->flushdb();
}
BENCHMARK(BM_createUniDB_10000)->Iterations(10)->Repetitions(5);

static void BM_createUniDB_100000(benchmark::State& state) {
    redis->flushdb();
    for (auto _ : state) {
        redis->createUniDB("../data_model.json", 100000);
    }
    redis->flushdb();
}
BENCHMARK(BM_createUniDB_100000)->Iterations(5)->Repetitions(5);

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