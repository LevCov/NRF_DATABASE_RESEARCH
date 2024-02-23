#pragma once

template<typename Create, typename Read, typename Update, typename Del_Read, typename Find_par, typename Find_ret>
class DBInterface {
public:
    virtual Find_ret find(const Find_par& filter) = 0;

    virtual bool create(const Create& document) = 0;

    virtual Read read(const Del_Read& val) = 0;

    virtual bool update(const Update& filter, const Update& update) = 0;

    virtual void del(const Del_Read& filter) = 0;

};


