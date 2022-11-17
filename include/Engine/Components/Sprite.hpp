#pragma once
#include "IComponent.hpp"
#include "Drawable.hpp"
#include "Entity.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Sprite : public IComponent, public Drawable {
    public:
        Sprite(Entity *self, sf::Texture const& texture,
            float const& width, float const& height);
        Sprite(Entity *self, std::string const& texturePath,
              float const& width, float const& height);
        ~Sprite() = default;

        sf::Sprite& getSprite();
        sf::Texture& getTexture();

        Sprite& setSprite(sf::Sprite const& sprite);
        Sprite& setTexture(sf::Texture const& texture);
        Sprite& setTexture(std::string const &filePath);
        Sprite& setTextureRect(sf::IntRect const& rect);
        Sprite& setTransform(Transform2D *transform);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            target.draw(_sprite, states);
        }

        void destroy() override final;

    private:
        sf::Texture _texture;
        sf::Sprite _sprite;
        Entity *_self;
};
