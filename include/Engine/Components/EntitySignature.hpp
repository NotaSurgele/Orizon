#pragma once
#include <string>

#include "IComponent.hpp"

class EntitySignature : public IComponent {
public:
    EntitySignature(Entity *e, std::string const& value) : _signature(value) {};
    ~EntitySignature() = default;

    std::string const& signature() { return _signature; }

    void destroy() override final {};

private:
    const std::string _signature;
};