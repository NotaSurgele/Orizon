#include "Raytracer.hpp"

RayTracer::RayTracer(const sf::Vector2f& position, 
                    const sf::Vector2f& direction) 
                    :   _position(position),
                        _direction(direction)
{
    _shape = sf::RectangleShape();
    _shape.setSize(sf::Vector2f(_direction.x * 300, _direction.y * 10));
    _shape.setFillColor(sf::Color::Red);
    _shape.setOutlineThickness(1);
    _shape.setOutlineColor(sf::Color::Red);
    _shape.setPosition(_position.x, _position.y);
}

bool RayTracer::hit(const sf::RectangleShape& wall, sf::Vector2i rotate)
{
    // const float PI = 3.14159265;

    // float dx = _shape.getPosition().x - (-rotate.x);
    // float dy = _shape.getPosition().y - (-rotate.y);

    // float rotation = (std::atan2(dy, dx)) * 180 / PI;
    
    // _shape.setRotation(rotation);
    // _direction = sf::Vector2(_shape.getSize().x * std::cos(rotation), _shape.getSize().y * std::sin(rotation));
    float wx_min = wall.getPosition().x;
    float wy_min = wall.getPosition().y;
    float sizex = wall.getSize().x;
    float sizey = wall.getSize().y;

    double tx1 = (wx_min - _shape.getPosition().x) / _direction.x;
    double tx2 = ((wx_min + sizex) - _shape.getPosition().x) / _direction.x;
    double ty1 = (wy_min - _shape.getPosition().y) / _direction.y;
    double ty2 = ((wy_min + sizey) - _shape.getPosition().y) / _direction.y;

    double tmin = std::max(std::min(tx1, tx2), std::min(ty1, ty2));
    double tmax = std::min(std::max(tx1, tx2), std::max(ty1, ty2));
    std::cout << "POSITION " << _position.x << " " << _position.y << std::endl;
    std::cout << " MAX " << tmax << " " << tmin << std::endl;
    if (tmax < 0.0f) {
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax) {
        return false;
    }

    if (tmin < 0.0f) {
        return true;
    }
    return true;
}

void RayTracer::show(const float& thickness)
{
    DRAW(_shape);
}