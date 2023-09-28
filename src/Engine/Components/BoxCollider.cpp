#include "BoxCollider.hpp"
#include "Velocity.hpp"

BoxCollider::BoxCollider(   Entity *e,
                            sf::Vector2<float> const& position,
                            sf::Vector2<float> const& size,
                            const int& checkRange)
                            : _position(position),
                            _size(size),
                            _range(checkRange),
                            _e(e)
{
    float angle = 0;

    for (std::size_t i = 0; i < 4; i++) {
        RayCaster ray = RayCaster(e->getComponent<Transform2D>()->position,
                                    sf::Vector2f(1, 0), 100.0f);
        ray.rotate(angle += 45);
        _rays.push_back(ray);
    }
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
                            _size(size),
                            _e(e)
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

BoxCollider::Type BoxCollider::getType()
{
    return _type;
}

Entity * BoxCollider::collidingWithEntity()
{
    return collidingWith;
}

Entity * BoxCollider::attachedEntity()
{
    return _e;
}

bool BoxCollider::overlap(BoxCollider *box)
{
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

bool BoxCollider::intersect(BoxCollider *collider, BoxCollider& intersection)
{
    collidingWith = nullptr;
    isColliding = false;
    sf::Rect<float> box1 = sf::Rect<float>(this->_position, this->_size);
    sf::Rect<float> box2 = sf::Rect<float>(collider->getPosition(), collider->getSize());
    bool res = box1.intersects(box2, box1);
    sf::Vector2f pos = sf::Vector2f(box1.left, box1.top);
    sf::Vector2f size = sf::Vector2f(box1.width, box1.height);
    intersection._position = pos;
    intersection._size = size;

    if (res) {
        isColliding = true;
        collidingWith = collider->attachedEntity();
    }
    return res;
}

bool BoxCollider::overlap(BoxCollider *collider, Velocity<float> *velocity)
{
    auto box = collider;
    if (!box)
        return false;
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

    isColliding = _position.x < predicted_pos.x + size.x &&
                  _position.x + _size.x > predicted_pos.x &&
                  _position.y < predicted_pos.y + size.y &&
                  _position.y + _size.y > predicted_pos.y;
    if (isColliding)
        collidingWith = box->attachedEntity();
    return isColliding;
}
