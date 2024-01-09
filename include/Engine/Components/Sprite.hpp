#pragma once
#include "IComponent.hpp"
#include "Drawable.hpp"
#include "Entity.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Sprite : public IComponent, public Drawable {
    public:
        Sprite(Entity *self, sf::Texture& texture,
            float const& width=1, float const& height=1);
        Sprite(sf::Texture& texture);
/*        Sprite(Entity *self, std::string const& texturePath,
              float const& width=1, float const& height=1);*/
        Sprite(Entity *self, const std::string& textureName,
               float const& width=1, float const& height=1);

        ~Sprite() = default;

        sf::Sprite& getSprite();
        const sf::Texture* getTexture();
        Sprite& setSprite(sf::Sprite const& sprite);
        Sprite& setTexture(sf::Texture& texture);
        Sprite& setTexture(std::string const &filePath);
        Sprite& setTextureRect(sf::IntRect const& rect);
        Sprite& setTransform(Transform2D *transform);
        Sprite& setColor(const sf::Color& color);
        Sprite& setShadowIntensity(const float& intensity=.4f);
        Sprite& setLightApply(bool apply);
        Sprite& setScale(const float& x, const float& y);
        Sprite& setTextureName(const std::string& name)
        {
            _textureName = name;
            return *this;
        }

        void setPosition(const sf::Vector2f& position);
        void setPosition(const float& x, const float& y);

        bool isLightApply();

        const sf::Vector2f& getPosition();
        const sf::Vector2f& getScale();
        const sf::Color& getColor();
        const float& getShadowIntensity();
        std::string& getTextureName()
        {
            return _textureName;
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            target.draw(_sprite, states);
        }

        void destroy() override final;

    private:
        std::string _textureName;
        float _intensity = .4f;
        sf::Texture _texture;
        sf::Sprite _sprite;
        bool _light = false;
        Entity *_self;
};
