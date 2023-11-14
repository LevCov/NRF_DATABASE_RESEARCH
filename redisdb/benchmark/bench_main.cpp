#include <benchmark/benchmark.h>

#include "../include/redis_find.h"

auto redis = std::make_unique<RedisInterface>("tcp://127.0.0.1:6379");

static void BM_Create(benchmark::State& state) {
    redis->flushdb();
    for (auto _ : state) {
        redis->set("1", "1");
    }
    redis->flushdb();
}
BENCHMARK(BM_Create);

static void BM_Read(benchmark::State& state) {
    redis->flushdb();
    redis->create("1", "1");
    for (auto _ : state) {
        redis->read("1");
    }
    redis->flushdb();
}
BENCHMARK(BM_Read);

static void BM_Del(benchmark::State& state) {
    redis->flushdb();
    redis->create("1", "1");
    for (auto _ : state) {
        redis->del("1");
    }
    redis->flushdb();
}
BENCHMARK(BM_Del);

static void BM_loadingDB(benchmark::State& state) {
    redis->flushdb();
    for (auto _ : state) {
        redis->loadingDB("data_model.json");
    }
    redis->flushdb();
}
BENCHMARK(BM_loadingDB);

static void BM_createDB_1(benchmark::State& state) {
    redis->flushdb();
    for (auto _ : state) {
        redis->createDB("data_model.json", 1, "NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_createDB_1);

static void BM_find_1(benchmark::State& state) {
    redis->flushdb();
    redis->createDB("data_model.json", 1, "NRF");
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_1);

static void BM_find_10(benchmark::State& state) {
    redis->flushdb();
    redis->createDB("data_model.json", 10, "NRF");
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_10);

static void BM_find_100(benchmark::State& state) {
    redis->flushdb();
    redis->createDB("data_model.json", 100, "NRF");
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_100);

static void BM_find_1000(benchmark::State& state) {
    redis->flushdb();
    redis->createDB("data_model.json", 1000, "NRF");
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_1000);

static void BM_find_10000(benchmark::State& state) {
    redis->flushdb();
    redis->createDB("data_model.json", 10000, "NRF");
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_10000);

static void BM_find_20000(benchmark::State& state) {
    redis->flushdb();
    redis->createDB("data_model.json", 20000, "NRF");
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_20000);

static void BM_find_40000(benchmark::State& state) {
    redis->flushdb();
    redis->createDB("data_model.json", 40000, "NRF");
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_40000);

static void BM_find_60000(benchmark::State& state) {
    redis->flushdb();
    redis->createDB("data_model.json", 60000, "NRF");
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_60000);

static void BM_find_80000(benchmark::State& state) {
    redis->flushdb();
    redis->createDB("data_model.json", 80000, "NRF");
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_80000);

static void BM_find_100000(benchmark::State& state) {
    redis->flushdb();
    redis->createDB("data_model.json", 100000, "NRF");
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_100000);

static void BM_find_400000(benchmark::State& state) {
    redis->flushdb();
    redis->createDB("data_model.json", 400000, "NRF");
    for (auto _ : state) {
        redis->find("NRF");
    }
    redis->flushdb();
}
BENCHMARK(BM_find_400000);

BENCHMARK_MAIN();
