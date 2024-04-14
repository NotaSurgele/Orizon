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
    sf::Texture textureCpy;
    std::size_t textureId = 0;
    sf::VertexArray vertexArray;
    std::size_t counter = 1;
};
