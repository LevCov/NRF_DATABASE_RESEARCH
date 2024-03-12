#include "RocksDBInterface.h"
#include "../UUID/UUIDv4.h"

using ROCKSDB_NAMESPACE::ReadOptions;


RocksDBInterface::RocksDBInterface(const char* url){
    Options options;
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();
    options.create_if_missing = true;

    auto s = DB::Open(options, url, &conn);
    std::vector<std::string> nfTypes {"NRF", "UDM", "AMF", "SMF", "AUSF", 
                                          "NEF", "PCF", "SMSF", "NSSF", "UDR", 
                                          "LMF", "GMLC", "5G_EIR", "SEPP", "UPF",
                                          "N3IWF", "AF", "UDSF", "BSF", "CHF", 
                                          "NWDAF"};
    for(auto nf: nfTypes){
        ColumnFamilyHandle *cf;
        s = conn->CreateColumnFamily(ColumnFamilyOptions(), nf, &cf);
        assert(s.ok());
        s = conn->DestroyColumnFamilyHandle(cf);
        assert(s.ok());
    }
    delete conn;

    std::vector<ColumnFamilyDescriptor> column_family;

    column_family.push_back(ColumnFamilyDescriptor(
        ROCKSDB_NAMESPACE::kDefaultColumnFamilyName, ColumnFamilyOptions()
    ));
    for(auto nf: nfTypes){
        column_family.push_back(ColumnFamilyDescriptor(
            nf, ColumnFamilyOptions()
        ));
    }

    s = DB::Open(DBOptions(), url, column_family, &handles, &conn);

    // std::vector<std::string> cf;
    // conn->ListColumnFamilies(DBOptions(), url, &cf);
    // for(auto i: cf){
    //     std::cout << i << std::endl;
    // }
    //std::cout << s.getState() << std::endl;
    assert(s.ok());
}

bool RocksDBInterface::create(const Create& par){
    auto s = conn->Put(WriteOptions(), handles[par.handle_n], par.key, par.value);
    return s.ok();
}

Read RocksDBInterface::read(const Del_Read& par){
    Read val;
    conn->Get(ReadOptions(), handles[par.handle_n] ,par.key, &val.value);
    return val;
}

bool RocksDBInterface::update(const Update& filter, const Update& update){
    std::string val;
    auto s = conn->Get(ReadOptions(), handles[filter.handle_n], filter.key, &val);
    if (s.ok()){
        conn->Put(WriteOptions(), handles[filter.handle_n],filter.key, update.value);
    }
    return !s.ok();
}

void RocksDBInterface::del(const Del_Read& par) {
    conn->SingleDelete(WriteOptions(), handles[par.handle_n], par.key);
}

FindRet RocksDBInterface::find(const FindPar& par) {
    rocksdb::Iterator* it = conn->NewIterator(ReadOptions(), handles[par.handle_n]);
    FindRet res;
    for (it->SeekToFirst(); it->Valid(); it->Next()){
        res.values.push_back(it->key().ToString());
        //std::cout << it->value().ToString();
    }
    delete it;
    return res;
}

void RocksDBInterface::createUniDB(const char *path, int n){
    std::ifstream cf(path); 

    if (cf.is_open()) {
        std::vector<std::string> nfTypes {"NRF", "UDM", "AMF", "SMF", "AUSF", 
                                          "NEF", "PCF", "SMSF", "NSSF", "UDR", 
                                          "LMF", "GMLC", "5G_EIR", "SEPP", "UPF",
                                          "N3IWF", "AF", "UDSF", "BSF", "CHF", 
                                          "NWDAF"};

        std::mt19937 rng;
        const unsigned int seed = 42;
        rng.seed(seed);
        std::uniform_int_distribution<int> dist(0, 20);
        json instances = json::parse(cf);  
        
        for (auto& profile : instances) {  // for 1 of 1. 
            for (int i = 0; i < n; i++) {
                std::string nfInstanceId = std::to_string(i);
                //std::string nfInstanceId = UUID::New().String();
                const int randomNumber = dist(rng);
                profile["nfInstanceId"] = nfInstanceId;
                profile["nfType"] = nfTypes[randomNumber];
                create(Create{
                    NF[nfTypes[randomNumber]],
                    nfInstanceId,
                    profile.dump()
                });
            }
        }

        cf.close();   
    }
}

void RocksDBInterface::flushdb(){
    for(size_t i = 0; i < NF.size() + 1; i++){
        rocksdb::Iterator* it = conn->NewIterator(ReadOptions(), handles[i]);
        FindRet res;
        for (it->SeekToFirst(); it->Valid(); it->Next()){
            conn->SingleDelete(WriteOptions(), it->key());
        }
        delete it;
    }
}

RocksDBInterface::~RocksDBInterface(){
    for (auto handle : handles) {
        conn->DropColumnFamily(handle);
        auto s = conn->DestroyColumnFamilyHandle(handle);
        assert(s.ok());
    }
    delete conn;
}