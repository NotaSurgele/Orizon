#include "Sprite.hpp"
#include "Transform2D.hpp"
#include "Core.hpp"
#include <iostream>

Sprite::Sprite( Entity *self,
                sf::Texture& texture,
                float const& scaleX,
                float const& scaleY) :  _sprite(),
                                        _texture(),
                                        _self(self)
{
    setTexture(texture);
    _sprite.setScale(scaleX, scaleY);
}

/*Sprite::Sprite( Entity *self,
                std::string const& texturePath,
                float const& scaleX,
                float const& scaleY) : _sprite(),
                                       _texture(),
                                       _self(self)
{
    setTexture(texturePath);
    _sprite.setScale(scaleX, scaleY);

}*/

Sprite::Sprite(Entity *self, const std::string& textureName, const float& w, const float& h) : _self(self),
                                                                                                _sprite(),
                                                                                                _texture()
{
    sf::Texture text = R_GET_RESSOURCE(sf::Texture, textureName);
    setTexture(text);
    _sprite.setScale(w, h);
}

Sprite::Sprite(sf::Texture& texture) : _self(nullptr)
{
    setTexture(texture);
}

sf::Sprite& Sprite::getSprite()
{
    return _sprite;
}

const sf::Texture* Sprite::getTexture()
{
    return _sprite.getTexture();
}

const sf::Vector2f& Sprite::getPosition()
{
    return this->_sprite.getPosition();
}

const sf::Vector2f& Sprite::getScale()
{
    return _sprite.getScale();
}

const float& Sprite::getShadowIntensity()
{
    return _intensity;
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

Sprite& Sprite::setScale(const float& x, const float& y)
{
    _sprite.setScale(x, y);
    return *this;
}

Sprite& Sprite::setShadowIntensity(const float &intensity)
{
    _intensity = intensity;
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

void Sprite::setPosition(const sf::Vector2f &position)
{
    _sprite.setPosition(position);
}

void Sprite::setPosition(const float& x, const float& y)
{
    _sprite.setPosition(x, y);
}

const sf::Color& Sprite::getColor()
{
    return _sprite.getColor();
}

bool Sprite::isLightApply()
{
    return _light;
}

Sprite& Sprite::setTexture(sf::Texture& texture)
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