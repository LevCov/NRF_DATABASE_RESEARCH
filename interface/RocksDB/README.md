# RocksDB
Взята официальная библиотека rocksdb

Скачивание пакета:
```
sudo apt-get - y install librocksdb-dev
```
Также скачивание дополнительного пакета для работы с json файлами:
```
sudo apt-get -y install nlohmann-json3-dev
```
## Rockspackage
Импорт необходимых библиотек и подключение пространств имен
## CRUDparams
Определение параметров CRUD методов и метода find
## RocksDBInterface
Интерфейса взаимодействия с базой данных RocksDB

Основные методы:

CRUD:
```
create
read
update
del
```
Создание базы из NFProfile's:
```
createUniDB
# path - путь к шаблону
# n    - кол-во NFProfile
```

Очистка БД:
```
flushdb
```

Главный метод поиска:

```
find
```

## CMake
В папке расположен CMake файл для сборки пакета