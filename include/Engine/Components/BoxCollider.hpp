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
        FALSE=0,
        TRUE=1
    };

    enum Type {
        DYNAMIC=0,
        STATIC=1,
        NUL=2,
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

    void destroy() override final { _colliderSystem.clear(); }

    bool overlap(BoxCollider *collider);
    bool overlap(BoxCollider *collider, Velocity *velocity);
    bool intersect(BoxCollider *collider, BoxCollider& intersections);

    void setPosition(sf::Vector2<float> const& pos)
    {
        _shape.setPosition(pos);
        _position = pos;
    }

    void setType(const Type& type);

    void setTrigger(bool isTrigger)
    {
        _isTrigger = isTrigger;
    }

    void setPosition(const float& x, const float& y)
    {
        float angle = 0;
        sf::Vector2f position = sf::Vector2f(x, y);

        _shape.setPosition(x, y);
        _position.x = x;
        _position.y = y;
    }

    void setSize(const sf::Vector2<float>& size)
    {
        _size = size;
    }

    Entity * collidingWithEntity();
    Entity * entity();

    sf::RectangleShape shape(const sf::Color& color);

    sf::Vector2<float> &getPosition();

    sf::Vector2<float> &getSize();

    sf::Vector2<float>& getOffset()
    {
        return _offsetPosition;
    }

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

    void draw(const bool& draw)
    {
        _draw = draw;
    }

    void registerSide(const Side& side)
    {
        auto containType = hasSide(side);

        if (containType)
            return;
        sides.push_back(side);
    }

    void setOffset(const sf::Vector2f& offset)
    {
        _offsetPosition = offset;
    }

    BoxCollider *onCollision(const std::function<void(BoxCollider *)>& system);
    BoxCollider *onTrigger(const std::function<void(BoxCollider *)>& system);

    std::vector<Side>& getSides()
    {
        return sides;
    }

    bool hasSide(const Side& side)
    {
        auto it = std::find(sides.begin(), sides.end(), side);

        return it != sides.end();
    }

    bool isTriggered()
    {
        return _isTrigger;
    }

    bool& shouldDraw()
    {
        return _draw;
    }

    const std::vector<std::function<void(BoxCollider *)>>& getColliderSystem()
    {
        return _colliderSystem;
    }

public:
    Collide collide = Collide::FALSE;
    Side side = Side::NONE;
    std::vector<Side> sides;
    bool isColliding = false;
    Entity *collidingWith;
    bool ___isSet = false;

private:
    sf::Vector2<float> _offsetPosition;
    sf::Vector2<float> _position;
    sf::Vector2<float> _size;
    sf::RectangleShape _shape;
    Type _type = Type::NUL;
    Entity *_e = nullptr;
    std::vector<std::function<void(BoxCollider *)>> _colliderSystem;
    std::vector<std::function<void(BoxCollider *)>> _triggerSystem;
    int _range = 0;
    bool _isTrigger = false;
    bool _draw = false;
};