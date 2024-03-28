#pragma once

#include <map>
#include <functional>
#include <string>
#include <iostream>
#include <list>

#define SIGNATURE(type) \
    typeid(type).name()

class Entity;

class IComponent {
public:
    IComponent() = default;
    ~IComponent() = default;

    void setSignature(const std::string& signature)
    {
        _signature = signature;
    }

    const std::string getSignature() const
    {
        return _signature;
    }

    virtual void destroy() = 0;
protected:
    std::string _signature;
};