**Тестирование созданный бенчмарков**

1.Запуск каждый отедльно.

1.1 Запуск **createUniDB_1000()**.
Описание:


 Бенчмарк создания базы данных состоящей из 1000 документов на основе data_model с измененными nfType.
 Так же идет я сделал нумерацию документов в поле nfInstanceId.
 Создание одного документа на основе data_model считается за 1 итерацию из 1000.
 Тестируется метод InsertDocument для вставки data_model и UpdateOne для изменения nfType и nfInstanceId.

 Результат:

 ![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/9ca58cc5-6c71-489c-bbd8-bbaa2798ce34)
  
  Создалась база данных Unidb с коллекцией Unicol в которой находится 1000 документов на основе data_model с измененными nfType.

  ![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/369059de-77c4-4aa9-8eab-460a383cfc70)
