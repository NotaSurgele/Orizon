#pragma once
#include <string>

#include "IComponent.hpp"

class Tag : public IComponent {
public:
    Tag(Entity *e, std::string const& value);

    ~Tag() = default;

    std::string const& value() const { return _signature; }

    bool equals(const std::string& val)
    {
        return val.find(_signature) != std::string::npos;
    }

    void setValue(const std::string& v);

    void destroy() override final {};

private:
    std::string _signature;
};