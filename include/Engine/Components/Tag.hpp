#pragma once
#include <string>

#include "IComponent.hpp"

class Tag : public IComponent {
public:
    Tag(Entity *e, std::string const& value) : _signature(value) {};
    ~Tag() = default;

    std::string const& value() const { return _signature; }

    bool equals(const std::string& val)
    {
        return val.find(_signature) != std::string::npos;
    }

    void destroy() override final {};

private:
    const std::string _signature;
};