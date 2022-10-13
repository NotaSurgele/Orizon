#include "Engine/Entity.hpp"


Entity& Entity::setPosition(sf::Vector2<float> const &position)
{
    _position = position;
    return *this;
}

Entity& Entity::setSize(sf::Vector2<float> const& size)
{
    _size = size;
    return *this;
}

Entity& Entity::setSprite(const sf::Sprite &sprite)
{
    _sprite = sprite;
    return *this;
}

Entity& Entity::setTexture(const sf::Texture &texture)
{
    _texture = texture;
    return *this;
}
