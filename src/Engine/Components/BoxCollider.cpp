#include "BoxCollider.hpp"

BoxCollider::BoxCollider (Entity *e, sf::Vector2<float> const& position,
                         sf::Vector2<float> const& size)
                         : _position(position),
                           _size(size)
{
    _shape = sf::RectangleShape();
    _shape.setSize(size);
    _shape.setFillColor(sf::Color::Transparent);
    _shape.setOutlineColor(color);
    _shape.setOutlineThickness(1.0f);
}

sf::Vector2<float> &BoxCollider::getPosition()
{
    return _position;
}

sf::Vector2<float> &BoxCollider::getSize()
{
    return _size;
}

bool BoxCollider::overlap(BoxCollider *collider)
{
    auto box = collider;
    auto pos = box->getPosition();
    auto size = box->getSize();

    auto max_x = _position.x + _size.x;
    auto max_y = _position.y + _size.y;
    if (!box) return false;
    return _position.x < pos.x + size.x &&
           _position.x + _size.x > pos.x &&
           _position.y < pos.y + size.y &&
           _position.y + _size.y > pos.y;
 }
