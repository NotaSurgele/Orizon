#include "Sprite.hpp"
#include <iostream>

Sprite::Sprite( Entity *self,
                sf::Texture const& texture,
                float const& scaleX,
                float const& scaleY) :  _sprite(),
                                        _texture(),
                                        _self(self)
{
    setTexture(texture);
    _sprite.setScale(scaleX, scaleY);
}

Sprite::Sprite( Entity *self,
                std::string const& texturePath,
                float const& scaleX,
                float const& scaleY) : _sprite(),
                                       _texture(),
                                       _self(self)
{
    setTexture(texturePath);
    _sprite.setScale(scaleX, scaleY);
}


sf::Sprite& Sprite::getSprite()
{
    return _sprite;
}

sf::Texture& Sprite::getTexture()
{
    return _texture;
}

const sf::Vector2f& Sprite::getPosition()
{
    return this->_sprite.getPosition();
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

Sprite& Sprite::setLightApply(bool apply)
{
    _light = apply;
    return *this;
}

Sprite& Sprite::setTransform(Transform2D *transform)
{
    if (transform != nullptr) {
        _sprite.setPosition(transform->position.x, transform->position.y);
        _sprite.setScale(transform->scale.x, transform->scale.y);
        _sprite.setRotation(transform->rotation);
    }
    return *this;
}

const sf::Color& Sprite::getColor()
{
    return _sprite.getColor();
}

bool Sprite::isLightApply()
{
    return _light;
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