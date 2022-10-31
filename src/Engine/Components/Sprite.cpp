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
                float const& height)
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

void Sprite::update()
{
    Transform2D *selfTransform = _self->Transform();

    // std::cout << selfTransform->position.x << std::endl;
    _sprite.setPosition(selfTransform->position);
    _sprite.setRotation(selfTransform->rotation);
}

void Sprite::destroy()
{
    _sprite.~Sprite();
    _texture.~Texture();
}
