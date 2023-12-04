#pragma once

#include "Engine/Entity.hpp"

class Gravity : public IComponent {
public:
    Gravity(Entity *e, double const& value=9.81f) : force(value) {}
    ~Gravity() = default;
    void destroy() override final {};

public:
    const float force = 9.81f;
    float trailingSpeed = 0.0f;
};