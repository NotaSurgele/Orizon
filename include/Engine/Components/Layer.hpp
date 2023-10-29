#pragma once

#include "IComponent.hpp"

class Layer : public IComponent {
public:
    Layer(Entity *self, std::size_t const& layer) : _layer(layer), _self(self) {}
    ~Layer() = default;

    std::size_t value() const {
        return _layer;
    }

    void set(std::size_t const& value);

    void destroy() override final { delete this; };

private:
    std::size_t _layer = 0;
    Entity *_self = nullptr;
};