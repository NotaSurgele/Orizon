#pragma once

#include "IComponent.hpp"
#include "Drawable.hpp"

class BoxCollider : public IComponent, public Drawable {
public:

    enum Collide {
        TRUE,
        FALSE
    };

    sf::Color color = sf::Color::Red;

    BoxCollider(Entity *e, sf::Vector2<float> const& position,
                sf::Vector2<float> const& size);

    ~BoxCollider() = default;

    void destroy() override final {}

    bool overlap(BoxCollider *collider);

    void setPosition(sf::Vector2<float> const& pos)
    {
        _shape.setPosition(pos);
        _position = pos;
    }

    void setState(Collide const& state)
    {
        _collide = state;
    }

    Collide getState()
    {
        return _collide;
    }

    sf::Vector2<float> &getPosition();

    sf::Vector2<float> &getSize();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(_shape, states);
    }

private:
    Collide _collide = Collide::FALSE;
    sf::Vector2<float> _position;
    sf::Vector2<float> _size;
    sf::RectangleShape _shape;
};

