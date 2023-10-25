#include "MongoDBinterface.h"


int main() {



    MongoDBConnector connector("mongodb://localhost:27017");
    mongocxx::instance instance{};


    /*
    // Пример обновления документа
    // Фильтр для поиска документа, который вы хотите обновить
    bsoncxx::builder::stream::document filter;
    filter << "key1" << "1";

    // Документ с обновлениями
    bsoncxx::builder::stream::document update;
    update << "$set" << bsoncxx::builder::stream::open_document
        << "key11" << "11"
        << bsoncxx::builder::stream::close_document;


    connector.UpdateDocument("mydb", "mycollection", filter.extract(), update.extract());


    */






    /*
    // Пример добавления документа
    bsoncxx::builder::basic::document doc_builder;
    doc_builder.append(bsoncxx::builder::basic::kvp("key1", "1"));
    connector.InsertDocument("mydb", "mycollection", doc_builder.extract());


    doc_builder.append(bsoncxx::builder::basic::kvp("key2", "2"));
    connector.InsertDocument("mydb", "mycollection", doc_builder.extract());

    doc_builder.append(bsoncxx::builder::basic::kvp("key3", "3"));
    connector.InsertDocument("mydb", "mycollection", doc_builder.extract());

    doc_builder.append(bsoncxx::builder::basic::kvp("key4", "4"));
    connector.InsertDocument("mydb", "mycollection", doc_builder.extract());
    */



    /*
     // Пример поиска документа
     bsoncxx::builder::basic::document filter_builder;
     filter_builder.append(bsoncxx::builder::basic::kvp("key1", "value1"));
     bsoncxx::document::value data = filter_builder.extract();

     auto a = connector.FindDocuments("mydb", "mycollection", filter_builder.view());

     //std::cout << typeid(a).name() << std::endl;
     */



     /*

     bsoncxx::builder::basic::document filter;
     filter.append(bsoncxx::builder::basic::kvp("key1", "1"));

     bsoncxx::builder::basic::document update_doc;
     update_doc.append(bsoncxx::builder::basic::kvp("$set",bsoncxx::builder::stream::open_document<< "field_to_update" << "new_value" <<bsoncxx::builder::stream::close_document);

     connector.UpdateDocument("mydb", "mycollection", filter.extract(), update_doc.extract());



     // Пример удаления документа
     bsoncxx::builder::basic::document filter_builder;
     filter_builder.append(bsoncxx::builder::basic::kvp("LEv", "lev"));
     connector.DeleteDocument("mydb", "mycollection", filter_builder.view());
     */
    return 0;
}
