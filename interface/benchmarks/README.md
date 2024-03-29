# Benchmarks
Бенчмарки реализованы для CRUD методов, метода find и комбинации методов find_and_update (так как в NRF в основном используются именно эти методы)

Для каждой БД написан CMake файл

Для запуска теста в папке benchmarks/\*DB\*:
```
mkdir build && cd build
cmake .. && make
./bench
```
С выводом в файл:
```
./bench --benchmark_out=/home/anx_tl/bench_data/rocks_bench.csv --benchmark_out_format=csv                                                                                                               ─╯
```

Файл data_model.json содержит json объект NFProfile, который будет использоваться для заполнения БД