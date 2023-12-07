#pragma once

#include "Entity.hpp"
#include <SFML/System.hpp>
class IComponent;

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
        return new Velocity();
    }

    void setX(float x) { _vel.x = x; }
    void setY(float y) { _vel.y = y; }

    void reset()
    {
        _vel.x = 0;
        _vel.y = 0;
    }

    const float getX() { return _vel.x; }
    const float getY() { return _vel.y; }

    const sf::Vector2f& values() { return _vel; }

private:
    sf::Vector2<float> _vel = sf::Vector2f(0, 0);
    static inline bool _is_zero = false;
};