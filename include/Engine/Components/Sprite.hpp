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
            float const& width=1, float const& height=1);
        Sprite(Entity *self, std::string const& texturePath,
              float const& width=1, float const& height=1);
        ~Sprite() = default;

        sf::Sprite& getSprite();
        sf::Texture& getTexture();
        Sprite& setSprite(sf::Sprite const& sprite);
        Sprite& setTexture(sf::Texture const& texture);
        Sprite& setTexture(std::string const &filePath);
        Sprite& setTextureRect(sf::IntRect const& rect);
        Sprite& setTransform(Transform2D *transform);
        Sprite& setColor(const sf::Color& color);
        Sprite& setLightApply(bool apply);

        bool isLightApply();

        const sf::Vector2f& getPosition();

        const sf::Color& getColor();

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            target.draw(_sprite, states);
        }

        void destroy() override final;

    private:
        sf::Texture _texture;
        sf::Sprite _sprite;
        bool _light = false;
        Entity *_self;
};
