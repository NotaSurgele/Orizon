#include "RayCaster.hpp"
#include "Math.hpp"
#include "Core.hpp"
#include "System.hpp"

RayCaster::RayCaster(const sf::Vector2f& position,
                    const sf::Vector2f& direction,
                    const float& length)
                    :   _position(position),
                        _length(length)
{
    _preciseDirection.x = direction.x;
    _preciseDirection.y = direction.y;
    sf::Vector2f pos2 = sf::Vector2f(_position.x + (_preciseDirection.x * length), _position.y + (_preciseDirection.y * length));
    _line[0].position = _position;
    _line[0].color = sf::Color::Red;
    _line[1].position = pos2;
    _line[1].color = sf::Color::Red;
}

void RayCaster::rotate(const float& angle)
{
    sf::Vector2f startPoint = _line[0].position;
    sf::Vector2f endPoint = _line[1].position;

    float centerX = startPoint.x;
    float centerY = startPoint.y;

    float diffX = endPoint.x - centerX;
    float diffY = endPoint.y - centerY;

    float rotatedX = centerX + diffX * cos(angle) - diffY * sin(angle);
    float rotatedY = centerY + diffX * sin(angle) + diffY * cos(angle);

    _line[1].position = sf::Vector2f(rotatedX, rotatedY);
}

bool RayCaster::skip(Transform2D *other)
{
    auto colliderPosition = other->position;
    auto startPosition = _line[0].position;
    auto colliderDirection = Math::direction2i(startPosition, colliderPosition);

    return colliderDirection.x != _direction.x || colliderDirection.y != _direction.y;
}

float RayCaster::getLength()
{
    return _length;
}

bool RayCaster::hit(Transform2D *reflector)
{
    float wx_min = reflector->position.x;
    float wy_min = reflector->position.y;
    float sizex = reflector->size.x;
    float sizey = reflector->size.y;

    _preciseDirection = Math::direction<float>(_line[0].position, _line[1].position);
    _direction = Math::direction2i(_line[0].position, _line[1].position);

    if (this->skip(reflector)) {
        return false;
    }

    double tx1 = (wx_min - _line[0].position.x) / _preciseDirection.x;
    double tx2 = (wx_min + sizex - _line[0].position.x) / _preciseDirection.x;
    double ty1 = (wy_min - _line[0].position.y) / _preciseDirection.y;
    double ty2 = (wy_min + sizey - _line[0].position.y) / _preciseDirection.y;

    double tmin = std::max(std::min(tx1, tx2), std::min(ty1, ty2));
    double tmax = std::min(std::max(tx1, tx2), std::max(ty1, ty2));

    if (tmax < 0.0 || tmin > tmax) {
        _hit = false;
        return false;
    }
    _hit = true;
    _position = _line[0].position;
    _collisionPoint.x = _line[0].position.x + _preciseDirection.x * tmin;
    _collisionPoint.y = _line[0].position.y + _preciseDirection.y * tmin;
    return true;
}

void RayCaster::setPosition(const sf::Vector2i& position)
{
    _position.x = position.x;
    _position.y = position.y;
    sf::Vector2f pos2 = sf::Vector2f(_position.x + (_preciseDirection.x * _length), _position.y + (_preciseDirection.y * _length));
    _line[1].position = pos2;
    _line[0].position.x = position.x;
    _line[0].position.y = position.y;
}

void RayCaster::setPosition(const sf::Vector2f& position)
{
    _position.x = position.x;
    _position.y = position.y;
    sf::Vector2f pos2 = sf::Vector2f(_position.x + (_preciseDirection.x * _length), _position.y + (_preciseDirection.y * _length));
    _line[1].position = pos2;
    _line[0].position.x = position.x;
    _line[0].position.y = position.y;
}

void RayCaster::setDirection(const sf::Vector2f& direction)
{
    _direction.x = direction.x;
    _direction.y = direction.y;
}

sf::Vector2f RayCaster::getCollisionPoint()
{
    return _collisionPoint;
}

sf::Vector2f RayCaster::getPosition()
{
    return _position;
}

void RayCaster::show(const float& thickness)
{
    if (_hit) {
        _line[1].position.x = _collisionPoint.x;
        _line[1].position.y = _collisionPoint.y;
    }
    WindowInstance.getSFMLRenderWindow()->draw(_line, 2, sf::Lines);
}
