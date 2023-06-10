#pragma once
#include <SFML/Graphics.hpp>
#include "Core.hpp"

class RayTracer {
public:
    RayTracer() = delete;
    RayTracer(  const sf::Vector2f& position,
                const sf::Vector2f& direction);

    void show(const float& thickness);
    bool hit(sf::RectangleShape wall, sf::Vector2i rotate);

private:
    sf::Vector2f _position = sf::Vector2f(0, 0);
    sf::Vector2f _direction = sf::Vector2f(0, 0);
    sf::Vertex _line[2];
    bool _hit = false;
    sf::Vector2f _collisionPoint = sf::Vector2f(0, 0);
};