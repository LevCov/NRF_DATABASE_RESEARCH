

#include <iostream> 
#include <fstream> 
#include "UUIDv4.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <random>
using namespace rapidjson;


static auto CreateProfile() //Функция для создания профиля со случайными параметрами.
{



    // Открываем файл для чтения
    std::ifstream input_file("data_model.json");

    // Проверяем, успешно ли открыт файл


    // Считываем JSON из файла
    std::istreambuf_iterator<char> begin(input_file);
    std::istreambuf_iterator<char> end;
    std::string json_str(begin, end);

    // Закрываем файл
    input_file.close();

    // Парсим JSON
    Document document;
    document.Parse(json_str.c_str());

    


    std::random_device rd; 
    std::mt19937 gen(rd()); 

    std::uniform_int_distribution<int> distribution_1(0, 20);
    std::uniform_int_distribution<int> distribution_2(0, 1);
  
    


    const char* nfTypes[]{ "NRF",    "UDM",  "AMF",  "SMF",   "AUSF", "NEF",
                          "PCF",    "SMSF", "NSSF", "UDR",   "LMF",  "GMLC",
                          "5G_EIR", "SEPP", "UPF",  "N3IWF", "AF",   "UDSF",
                          "BSF",    "CHF",  "NWDAF" };  // 20 nftypes.




    const char* NFStatus[]{ "REGISTERED",    "UNDISCOVERABLE" };

     //Генерация случайного nfInstanceId
    std::string uuid = UUID::New().String();
    if (document[0].HasMember("nfInstanceId") && document[0]["nfInstanceId"].IsString()) {
        document[0]["nfInstanceId"].SetString(uuid.c_str(), static_cast<SizeType>(uuid.length()), document.GetAllocator());

    }

   

    //Генерация случайного nfType
    int random_number = distribution_1(gen);
    if (document[0].HasMember("nfType") && document[0]["nfType"].IsString()) {
        document[0]["nfType"].SetString(nfTypes[random_number], static_cast<SizeType>(strlen(nfTypes[random_number])), document.GetAllocator());

    }

    //Генерация случайного nfStatus
    random_number = distribution_2(gen);
    if (document[0].HasMember("nfStatus") && document[0]["nfStatus"].IsString()) {
        document[0]["nfStatus"].SetString(NFStatus[random_number], static_cast<SizeType>(strlen(NFStatus[random_number])), document.GetAllocator());

    }

    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    document.Accept(writer);

    return buffer; 
}
