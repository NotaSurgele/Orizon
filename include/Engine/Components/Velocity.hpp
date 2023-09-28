#pragma once

#include "Entity.hpp"
#include "IComponent.hpp"

template<typename T>
class Velocity : public IComponent {
public:
    Velocity(Entity *e) { }
    ~Velocity() = default;
    Velocity() = default;

    void destroy() override final
    {
        delete this;
    }

    void destroyIfZero()
    {
        if (_is_zero)
            delete this;
    }

    static inline Velocity* zero()
    {
        _is_zero = true;
        return new Velocity<T>();
    }

    void setX(T x) { _vel.x = x; }
    void setY(T y) { _vel.y = y; }

    void reset()
    {
        _vel.x = 0;
        _vel.y = 0;
    }

    const T getX() { return _vel.x; }
    const T getY() { return _vel.y; }

    const sf::Vector2<T>& values() { return _vel; }

private:
    sf::Vector2<T> _vel = sf::Vector2<T>(0, 0);
    static inline bool _is_zero = false;
};
