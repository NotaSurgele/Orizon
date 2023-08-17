#include "Sprite.hpp"
#include <iostream>

Sprite::Sprite( Entity *self,
                sf::Texture const& texture,
                float const& width,
                float const& height) :  _sprite(),
                                        _texture(),
                                        _self(self)
{
    setTexture(texture);
    _sprite.setScale(width, height);
}

Sprite::Sprite( Entity *self,
                std::string const& texturePath,
                float const& width,
                float const& height) : _sprite(),
                                       _texture(),
                                       _self(self)
{
    setTexture(texturePath);
    _sprite.setScale(width, height);
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

Sprite& Sprite::setColor(const sf::Color& color)
{
    _sprite.setColor(color);
    return *this;
}


Sprite& Sprite::setTransform(Transform2D *transform)
{
    if (transform != nullptr) {
        _sprite.setPosition(transform->position.x, transform->position.y);
        _sprite.setScale(transform->size.x, transform->size.y);
        _sprite.setRotation(transform->rotation);
    }
    return *this;
}

const sf::Color& Sprite::getColor()
{
    return _sprite.getColor();
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
    _sprite.setTexture(_texture);
    return *this;
}

Sprite& Sprite::setTextureRect(sf::IntRect const &rect)
{
    _sprite.setTextureRect(rect);
    return *this;
}

void Sprite::destroy()
{
    _sprite.~Sprite();
    _texture.~Texture();
}