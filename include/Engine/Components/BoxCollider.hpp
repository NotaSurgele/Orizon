#pragma once

#include "IComponent.hpp"
#include "Velocity.hpp"
#include "Drawable.hpp"
#include "Entity.hpp"

class BoxCollider : public IComponent, public Drawable {
public:

    enum Collide {
        TRUE,
        FALSE
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

    void setPosition(const float& x, const float& y)
    {
        _shape.setPosition(x, y);
        _position.x = x;
        _position.y = y;
    }

    sf::RectangleShape shape(const sf::Color& color);

    sf::Vector2<float> &getPosition();

    sf::Vector2<float> &getSize();

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
    int _range = 0;
};