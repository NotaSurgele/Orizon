#pragma once

#include "Entity.hpp"
#include "IComponent.hpp"

template<typename T>
class Velocity : public IComponent {
public:
    Velocity() = default;
    ~Velocity() = default;

    void destroy() override final {};

    static inline Velocity* zero()
    {
        if (!_self)
            _self = new Velocity<T>();
        return _self;
    }

    void setX(T x) { _vel.x = x; }
    void setY(T y) { _vel.y = y; }

    const T getX() { return _vel.x; }
    const T getY() { return _vel.y; }

private:
    static inline Velocity* _self;
    static inline sf::Vector2<T> _vel = sf::Vector2<T>(0, 0);
};
