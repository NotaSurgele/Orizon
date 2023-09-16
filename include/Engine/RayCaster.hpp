#pragma once
#include <SFML/Graphics.hpp>
#include "Transform2D.hpp"

class RayCaster {
public:
    RayCaster() = delete;
    RayCaster(  const sf::Vector2f& position,
                const sf::Vector2f& direction,
                const float& length);

    void setPosition(const sf::Vector2i& position);
    void setPosition(const sf::Vector2f& position);
    void show(const float& thickness);
    bool hit(Transform2D *other);
    void setDirection(const sf::Vector2f& direction);
    void rotate(const float& angle);
    sf::Vector2f getCollisionPoint();
    sf::Vector2f getPosition();

private:
    bool skip(Transform2D *collider);

private:
    sf::Vector2f _position = sf::Vector2f(0, 0);
    sf::Vector2i _direction = sf::Vector2i(0, 0);
    sf::Vector2f _preciseDirection = sf::Vector2f(0, 0);
    sf::Vertex _line[2];
    bool _hit = false;
    sf::Vector2f _collisionPoint = sf::Vector2f(0, 0);
    float _length = 1.0f;
};
