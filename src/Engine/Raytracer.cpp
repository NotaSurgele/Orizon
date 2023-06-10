#include "Raytracer.hpp"

RayTracer::RayTracer(const sf::Vector2f& position, 
                    const sf::Vector2f& direction) 
                    :   _position(position)
{
    _direction.x = direction.x;
    _direction.y = direction.y;
    sf::Vector2f pos2 = sf::Vector2f(_position.x + (_direction.x * 300), _position.y + (_direction.y * 300));
    _line[0].position = _position;
    _line[0].color = sf::Color::Red;
    _line[1].position = pos2;
    _line[1].color = sf::Color::Red;
}

bool RayTracer::hit(sf::RectangleShape wall, sf::Vector2i rotate)
{
    float wx_min = wall.getPosition().x;
    float wy_min = wall.getPosition().y;
    float sizex = wall.getSize().x;
    float sizey = wall.getSize().y;
    float x1 = _line[0].position.x;
    float y1 = _line[0].position.y;
    float x2 = _line[1].position.x;
    float y2 = _line[1].position.y;
    if (Input::isKeyPressed("Down")) {
        y2 += 400 * Time::deltaTime;
    }
    if (Input::isKeyPressed("Up")) {
        y2 -= 400 * Time::deltaTime;
    }
    if (Input::isKeyPressed("Left")) {
        x1 -= 400 * Time::deltaTime;
        x2 -= 400 * Time::deltaTime;
    }
    if (Input::isKeyPressed("Right")) {
        x1 += 400 * Time::deltaTime;
        x2 += 400 * Time::deltaTime;
    }
    sf::Vector2f dist = sf::Vector2f(x2-x1, y2-y1);
    float m = sqrt(dist.x*dist.x+dist.y*dist.y);
    _direction.x = dist.x/m;
    _direction.y = dist.y/m;
    _line[0].position.x = x1;
    _line[0].position.y = y1;
    _line[1].position.y = y2;
    _line[1].position.x = x2;

    std::cout << "Direction 1" << _direction.x << " " << _direction.y << std::endl;
    double tx1 = (wx_min - x1) / _direction.x;
    double tx2 = ((wx_min + sizex) - x1) / _direction.x;
    double ty1 = (wy_min - y1) / _direction.y;
    double ty2 = ((wy_min + sizey) - y1) / _direction.y;
    double tmin = std::max(std::min(tx1, tx2), std::min(ty1, ty2));
    double tmax = std::min(std::max(tx1, tx2), std::max(ty1, ty2));

    std::cout << tmin << " " << tmax << std::endl;
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
    Window.getSFMLRenderWindow().draw(_line, 2, sf::Lines);
}