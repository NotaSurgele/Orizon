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

    static inline Velocity* zero()
    {
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

private:
    sf::Vector2<T> _vel = sf::Vector2<T>(0, 0);
};
