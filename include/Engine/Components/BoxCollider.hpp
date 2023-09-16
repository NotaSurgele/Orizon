#pragma once

#include "IComponent.hpp"
#include "Velocity.hpp"
#include "Drawable.hpp"
#include "Entity.hpp"
#include "RayCaster.hpp"
#include <vector>

class BoxCollider : public IComponent, public Drawable {
public:

    enum Collide {
        TRUE,
        FALSE
    };

    enum Type {
        DYNAMIC = 0,
        STATIC = 1
    };

    enum Side {
        TOPLEFT,
        TOP,
        TOPRIGHT,
        RIGHT,
        DOWNRIGHT,
        DOWN,
        DOWNLEFT,
        LEFT,
        NONE
    };

    sf::Color color = sf::Color::Red;

    BoxCollider(Entity *e, sf::Vector2<float> const& position,
                sf::Vector2<float> const& size);

    BoxCollider(Entity *e, sf::Vector2<float> const& position,
                sf::Vector2<float> const& size, const int& range);

    ~BoxCollider() = default;

    void destroy() override final {}

    bool overlap(BoxCollider *collider);
    bool overlap(BoxCollider *collider, Velocity<float> *velocity);
    bool intersect(BoxCollider *collider, BoxCollider& intersections);

    void setPosition(sf::Vector2<float> const& pos)
    {
        _shape.setPosition(pos);
        _position = pos;
    }

    void setType(const Type& type)
    {
        _type = type;
    }

    void setPosition(const float& x, const float& y)
    {
        float angle = 0;
        sf::Vector2f position = sf::Vector2f(x, y);

        for (std::size_t i = 0; i < _rays.size(); i++) {
            _rays[i].setPosition(position);
            _rays[i].rotate(angle += 45);
            // _rays[i].show(2.0f);
        }
        _shape.setPosition(x, y);
        _position.x = x;
        _position.y = y;
    }

    sf::RectangleShape shape(const sf::Color& color);

    sf::Vector2<float> &getPosition();

    sf::Vector2<float> &getSize();

    Type getType();

    int& getRange();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(_shape, states);
    }

    void setColor(const sf::Color& color)
    {
        _shape.setOutlineColor(color);
    }

public:
    Collide state = Collide::FALSE;
    Side side = Side::NONE;

private:
    sf::Vector2<float> _position;
    sf::Vector2<float> _size;
    sf::RectangleShape _shape;
    std::vector<RayCaster> _rays;
    Type _type = Type::DYNAMIC;
    int _range = 0;
};