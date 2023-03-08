#pragma once

#include "IComponent.hpp"


class Layer : public IComponent {
public:
    Layer(Entity *self, std::size_t const& layer) : _layer(layer) {}
    ~Layer() = default;

    int value() {
        return _layer;
    }

    void set(std::size_t const& value) {
        _layer = value;
    }

    void destroy() override final { delete this; };

private:
    std::size_t _layer = 0;
};