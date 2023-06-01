#include "BoxCollider.hpp"
#include "Velocity.hpp"

BoxCollider::BoxCollider(   Entity *e,
                            sf::Vector2<float> const& position,
                            sf::Vector2<float> const& size,
                            const int& checkRange)
                            : _position(position),
                            _size(size),
                            _range(checkRange)
{
    _shape = sf::RectangleShape();
    _shape.setSize(size);
    _shape.setFillColor(sf::Color::Transparent);
    _shape.setOutlineColor(color);
    _shape.setOutlineThickness(1.0f);
}

BoxCollider::BoxCollider(   Entity *e,
                            sf::Vector2<float> const& position,
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

int& BoxCollider::getRange()
{
    return _range;
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

sf::RectangleShape BoxCollider::shape(const sf::Color& color)
{
    sf::RectangleShape rect = sf::RectangleShape();

    rect.setFillColor(sf::Color::Transparent);
    rect.setSize(this->_size);
    rect.setPosition(this->_position);
    rect.setOutlineColor(color);
    rect.setOutlineThickness(1.0f);
    return rect;
}

bool BoxCollider::overlap(BoxCollider *collider, Velocity<float> *velocity)
{
    auto box = collider;
    auto pos = box->getPosition();
    auto size = box->getSize();

    float valx = velocity->getX() > 0 ? 1 : velocity->getX() < 0 ? -1 : 0;
    float valy = velocity->getY() > 0 ? 1 : velocity->getY() < 0 ? -1 : 0;
    sf::Vector2<float> predicted_pos = sf::Vector2<float>(
        pos.x + valx,
        pos.y + valy
    );
    auto max_x = _position.x + _size.x;
    auto max_y = _position.y + _size.y;
    if (!box) return false;

    return _position.x < predicted_pos.x + size.x &&
           _position.x + _size.x > predicted_pos.x &&
           _position.y < predicted_pos.y + size.y &&
           _position.y + _size.y > predicted_pos.y;
}
