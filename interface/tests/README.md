# Тесты
Для каждой БД написан CMake файл.

Для запуска теста в папке tests/\*DB\*:
```
mkdir build && cd build
cmake .. && make
./utest
```

Папка data содержит json объект, который будет использоваться для заполнения БД