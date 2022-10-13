#include "Engine/Entity.hpp"


void Entity::setPosition(sf::Vector2<float> const &position)
{
    _position = position;
    return;
}

void Entity::setSize(sf::Vector2<float> const& size)
{
    _size = size;
    return;
}

void Entity::setSprite(const sf::Sprite &sprite)
{
    _sprite = sprite;
    return;
}

void Entity::setTexture(const sf::Texture &texture)
{
    _texture = texture;
    return;
}
