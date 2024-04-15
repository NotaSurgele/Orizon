#pragma once

#include <SFML/Graphics.hpp>
#include <Components/Sprite.hpp>

class SpriteBatch {
public:
    SpriteBatch();
    ~SpriteBatch() = default;

    void draw(Sprite *sprite);
    void clear();

    const sf::Texture *texture = nullptr;
    std::vector<sf::Vertex> vertexArray;
    sf::Texture textureCpy;
    std::size_t textureId = 0;
    std::size_t counter = 1;
};
