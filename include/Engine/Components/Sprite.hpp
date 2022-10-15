#pragma once
#include "IComponent.hpp"
#include "Entity.hpp"
#include <string>
#include <SFML/Graphics.hpp>

class Sprite : public IComponent {
    public:
        Sprite();
        ~Sprite() = default;

        sf::Sprite& getSprite();
        sf::Texture& getTexture();

        Sprite& setSprite(sf::Sprite const& sprite);
        Sprite& setTexture(sf::Texture const& texture);
        Sprite& setTexture(std::string const &filePath);

        void update() override final;

    private:
        sf::Texture _texture;
        sf::Sprite _sprite;
        Entity *_self;
};