**Тестирование созданныx бенчмарков**

1.Запуск каждого отедльно.

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




   









 1.2 Запуск **BM_UpdateDocument_1000()**.
 Описание:


Бенчмарк изменения nfType на значение UPDATED при условии, что изначально он был NRF
Изменения одного документа считается за 1 итерацию из 1000.
Тестируется метод UpdateOne для изменения nfType.

Результат:

![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/b67670b4-ed00-47e3-9446-a1f98e7c85eb)

У всех документов с nfType - NRF поменялся nfType на UPDATED.

![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/22c2343f-f2a0-46d4-ab37-bd9aff39f703)

![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/243abcdc-2613-4f87-ba90-cfc8ba796acd)

![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/ff92cc44-e9f0-4936-bcf3-9ebc3d13d76b)






1.3 Запуск **BM_FindDocument_1000()**.
 Описание:
Бенчмарк поиска документа по ключ - значение. 
Результат работы FindDocument передается в переменную result.Документы ищу по фильтру nfType: UPDATED.  Далее result преобрузуется в фильтр для метода UpdateOne,
с помощью которого я помечаю(поменялся nfType на FINDED) найденные документы.


Результат:

![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/32c6af8e-d300-45e9-850f-c717d781a475)

У всех документов найденных документов nfType поменялся на FINDED.


![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/f6c6af38-cce2-4b59-9585-0b06f25845ff)


![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/7adbcad3-96f0-4966-b488-6e53b22f5046)


**1.Запуск всех вместе.**


![image](https://github.com/LevCov/NRF_DATABASE_RESEARCH/assets/100800333/47efcaf5-330c-41a2-8fb8-40d58377f903)


