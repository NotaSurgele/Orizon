#include "Sprite.hpp"

Sprite::Sprite() : _sprite(),
                   _texture()
{
}

sf::Sprite& Sprite::getSprite()
{
    return _sprite;
}

sf::Texture& Sprite::getTexture()
{
    return _texture;
}

Sprite& Sprite::setSprite(sf::Sprite const& sprite)
{
    _sprite = sprite;
    return *this;
}

Sprite& Sprite::setTexture(sf::Texture const& texture)
{
    _texture = texture;
    _sprite.setTexture(_texture);
    return *this;
}

Sprite& Sprite::setTexture(std::string const &filePath)
{
    _texture.loadFromFile(filePath);
    return *this;
}

Sprite& Sprite::setTextureRect(sf::IntRect const &rect)
{
    _sprite.setTextureRect(rect);
    return *this;
}

void Sprite::update()
{
    Transform2D *selfTransform = _self->Transform();

    _sprite.setPosition(selfTransform->position);
    _sprite.setScale(selfTransform->size);
    _sprite.setRotation(selfTransform->rotation);
}
