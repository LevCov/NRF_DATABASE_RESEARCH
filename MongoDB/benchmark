
#include "benchmark/benchmark.h"
#include "C:\Users\levax\source\repos\ConsoleApplication1\ConsoleApplication1\MongoDBConnector.h"


#include <random>


 mongocxx::instance instance{};

 // Подключение к MongoDB
 mongocxx::client client{mongocxx::uri{"mongodb://localhost:27017"}};

 const char* database_name = "Test";
 const char* collection_name = "test_collection";
 MongoDBConnector connector{"mongodb://localhost:27017"};










 //Простые бенчмарки для проверки работоспособности 
 
 static void BM_Create(benchmark::State& state) {



   bsoncxx::document::value doc_value = bsoncxx::from_json("{\"1\": \"1\"}");

   for (auto _ : state) {
     connector.InsertDocument(database_name, collection_name, doc_value);
   }
 }
 BENCHMARK(BM_Create)->Iterations(100000)->Repetitions(5);


 


 static void BM_Read(benchmark::State& state) {


   bsoncxx::builder::stream::document filter{};
   filter << "1"<< "1";

   for (auto _ : state) {
     connector.FindDocument(database_name, collection_name, filter.view());
   }
 }

 BENCHMARK(BM_Read)->Iterations(100000)->Repetitions(5);
 

 static void BM_DeleteDocument(benchmark::State& state) {
   bsoncxx::builder::stream::document filter{};
   filter << "1"
          << "1";

   for (auto _ : state) {
     connector.DeleteDocument(database_name, collection_name, filter.view());
   }
 }
 BENCHMARK(BM_DeleteDocument)->Iterations(100000)->Repetitions(5);
 


 

 
  static void BM_Create(benchmark::State& state) {
   bsoncxx::document::value doc_value = bsoncxx::from_json("{\"1\": \"1\"}");

   for (auto _ : state) {
     connector.InsertDocument(database_name, collection_name, doc_value);
   }
 }
 BENCHMARK(BM_Create)->Iterations(100);
 

 
 static void BM_UpdateDocument(benchmark::State& state) {
  
   bsoncxx::builder::stream::document filter{};
   filter << "1"<< "10";

   bsoncxx::builder::stream::document update{};

   update << "$set" << bsoncxx::builder::stream::open_document<< "1"<< "1" << bsoncxx::builder::stream::close_document;
   bsoncxx::builder::stream::document filter_{};


   for (auto _ : state) {
     connector.UpdateOne(database_name, collection_name, filter.view(),update.view());

   }


 }
 BENCHMARK(BM_UpdateDocument)->Iterations(100);
 





 ////////////////////////////////////////////////////////////////////////////////////////////////////////

 /*
  Бенчмарк создания базы данных состоящей из 1000 документов на основе data_model с измененными nfType.
  Так же идет я сделал нумерацию документов в поле nfInstanceId.

  Создание одного документа на основе data_model считается за 1 итерацию из 1000.
 
  Тестируется метод InsertDocument для вставки data_model и UpdateOne для изменения nfType и nfInstanceId.

 */
 
 static void createUniDB_1000(benchmark::State& state) {

   const char* database_name = "Unidb";
   const char* collection_name = "Unicol";

   std::mt19937 rng;
   const unsigned int seed = 42;
   rng.seed(seed);
   std::uniform_int_distribution<int> dist(0, 20);

    const char* nfTypes[]{"NRF",
                         "UDM",
                         "AMF",
                         "SMF",
                         "AUSF",
                         "NEF",
                         "PCF",
                         "SMSF",
                         "NSSF",
                         "UDR",
                         "LMF",
                         "GMLC",
                         "5G_EIR",
                         "SEPP",
                         "UPF",
                         "N3IWF",
                         "AF",
                         "UDSF",
                         "BSF",
                         "CHF",
                         "NWDAF"};  // 20 nftypes.


   std::string jsonContent;
   std::ifstream jsonFile("data_model.json");

   if (jsonFile.is_open()) {
     std::getline(jsonFile, jsonContent, '\0');
     jsonFile.close();

   } 
   else {


     std::cerr << "Unable to open JSON file." << std::endl;
     return;
   }
   auto doc = bsoncxx::from_json(jsonContent);
   int i = 0;

   while (i < 1000) {

     connector.InsertDocument(database_name, collection_name, doc);
     i = i + 1;
     
   }

  
   
   i = 0;
   for (auto _ : state) {
    
       bsoncxx::builder::stream::document filter{};
     filter << "0.nfInstanceId"
            << "string";

     bsoncxx::builder::stream::document update{};

     const int randomNumber = dist(rng);
     update << "$set" << bsoncxx::builder::stream::open_document << "0.nfType"
            << nfTypes[randomNumber]
            << bsoncxx::builder::stream::close_document;
     bsoncxx::builder::stream::document filter_{};

     connector.UpdateOne(database_name, collection_name, filter.view(),
                         update.view());

     update << "$set" << bsoncxx::builder::stream::open_document
            << "0.nfInstanceId" << i
            << bsoncxx::builder::stream::close_document;
     connector.UpdateOne(database_name, collection_name, filter.view(),
                         update.view());
     i = i + 1;
   }




 }
 BENCHMARK(createUniDB_1000)->Iterations(1000);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 /*
  Бенчмарк изменения nfType на значение UPDATED при условии, что изначально он был NRF

  Изменения одного документа считается за 1 итерацию из 1000.

  Тестируется метод UpdateOne для изменения nfType .

 */
 
 static void BM_UpdateDocument_1000(benchmark::State& state) {
    

   const char* database_name = "Unidb";
   const char* collection_name = "Unicol";

   std::mt19937 rng;
   const unsigned int seed = 42;
   rng.seed(seed);
   std::uniform_int_distribution<int> dist(0, 20);
   const int randomNumber = dist(rng);
   
   const char* nfTypes[]{"NRF",    "UDM",  "AMF",  "SMF",   "AUSF", "NEF",
                         "PCF",    "SMSF", "NSSF", "UDR",   "LMF",  "GMLC",
                         "5G_EIR", "SEPP", "UPF",  "N3IWF", "AF",   "UDSF",
                         "BSF",    "CHF",  "NWDAF"};  // 20 nftypes.

   
  

   for (auto _ : state) {

     bsoncxx::builder::stream::document filter{};

     bsoncxx::builder::stream::document update{};

     const int randomNumber = dist(rng);
     filter << "0.nfType" << "NRF";   
     update << "$set" << bsoncxx::builder::stream::open_document << "0.nfType"
            << "UPDATED" << bsoncxx::builder::stream::close_document;   
     connector.UpdateOne(database_name, collection_name, filter.view(),update.view());



   }
 }
BENCHMARK(BM_UpdateDocument_1000)->Iterations(1000);



////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 Бенчмарк поиска документа по ключ - значения. 

 

 Результат работы FindDocument передается в переменную result. Далее result преобрузуется в фильтр для метода UpdateOne,
 с помощью которого я помечаю найденные документы.

 

*/

 static void BM_FindDocument_1000(benchmark::State& state) {
   const char* database_name = "Unidb";
   const char* collection_name = "Unicol";

   int f = 0;
   for (auto _ : state) {
        
       bsoncxx::builder::stream::document filter{};

       filter << "0.nfType"
              << "UPDATED";   

      
       bsoncxx::stdx::optional<bsoncxx::document::value> result =connector.FindDocument(database_name, collection_name, filter.view());
       
       if (result) {
       
         bsoncxx::document::view view = result->view();
         bsoncxx::builder::stream::document update;
         update << "$set" << bsoncxx::builder::stream::open_document << "0.nfType"
              << "FINDED" << bsoncxx::builder::stream::close_document;
         connector.UpdateOne(database_name, collection_name, view, update.view());

         f = f + 1;
       }
       
       
      
   }

   std::cout << f << std::endl;
 }

BENCHMARK(BM_FindDocument_1000)->Iterations(1000);
 
 








 



BENCHMARK_MAIN();


 
