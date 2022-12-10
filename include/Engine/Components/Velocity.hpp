#pragma once

#include "Entity.hpp"
#include "IComponent.hpp"

template<typename T>
class Velocity : public IComponent {
public:
    Velocity() = default;
    ~Velocity() = default;

    void destroy() override final {};

    void setX(T x) { _x = x; }
    void setY(T y) { _y = y; }

    const T getX() { return _x; }
    const T getY() { return _y; }

private:
    T _x;
    T _y;
};
