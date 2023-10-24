#pragma once

#include <memory>
#include <mutex>
#include <BD.h>

template<typename Conn>
class BDConnection{
public:
    virtual std::shared_ptr<Conn> connection() const = 0;
};