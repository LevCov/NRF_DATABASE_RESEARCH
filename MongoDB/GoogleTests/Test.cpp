#include "pch.h"
#include "MongoDBConnector.h"


class MongoDBConnectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Подготовка к тестированию, если необходимо
    }

    void TearDown() override {
        // Очистка после тестирования, если необходимо
    }

    MongoDBConnector connector{ "mongodb://localhost:27017" };
};



TEST_F(MongoDBConnectorTest, Create) {
    const char* database_name1 = "test_db1";
    const char* database_name2 = "test_db2";
    const char* collection_name = "test_collection";

    connector.Create(database_name1, collection_name);
    connector.Create(database_name2, collection_name);

    // Проверяем, что база данных и коллекция были созданы успешно
    // Можно использовать другие методы MongoDBConnector для этой проверки
}



TEST_F(MongoDBConnectorTest, Drop) {
    const char* database_name = "test_db1";

    
    connector.Drop(database_name);

    // Проверяем, что база данных была удалена успешно
    // Можно использовать другие методы MongoDBConnector для этой проверки
}


TEST_F(MongoDBConnectorTest, InsertDocument) {
    const char* database_name = "test_db2";
    const char* collection_name = "test_collection";
    //bsoncxx::document::value doc_value = bsoncxx::from_json("{\"key\": \"value\"}");

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

    // Разбираем JSON строку
    auto doc = bsoncxx::from_json(jsonContent);


    connector.InsertDocument(database_name, collection_name, doc);

    // Проверяем, что документ был успешно вставлен
    // Можно использовать другие методы MongoDBConnector для этой проверки
}


TEST_F(MongoDBConnectorTest, UpdateDocument) {
    const char* database_name = "test_db2";
    const char* collection_name = "test_collection";

    bsoncxx::builder::stream::document filter{};
    filter << "0.nfType" << "NRF";

    bsoncxx::builder::stream::document update{};
    
    update << "$set" << bsoncxx::builder::stream::open_document << "0.nfInstanceId" << "01234" << bsoncxx::builder::stream::close_document;
    bsoncxx::builder::stream::document filter_{};
    filter_ << "0.nfInstanceId" << "01234";

    connector.UpdateOne(database_name, collection_name, filter.view(), update.view());

}


TEST_F(MongoDBConnectorTest, DeleteDocument) {
    const char* database_name = "test_db2";
    const char* collection_name = "test_collection";

    bsoncxx::builder::stream::document filter{};
    filter << "key" << "value";

    bsoncxx::builder::stream::document update{};
    update << "$set" << bsoncxx::builder::stream::open_document << "0.nfInstanceId" << "01234" << bsoncxx::builder::stream::close_document;

    connector.DeleteDocument(database_name, collection_name, filter.view());

}




int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    mongocxx::instance instance{};
    return RUN_ALL_TESTS();
}


