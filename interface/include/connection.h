#pragma once

#include <memory>
#include <mutex>

template<typename Conn>
class BDConnection{
public:
    virtual std::shared_ptr<Conn> connection() const = 0;
};