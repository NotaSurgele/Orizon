#pragma once

#include <SFML/Graphics.hpp>
#include <Components/Sprite.hpp>

class SpriteBatch {
public:
    SpriteBatch();
    ~SpriteBatch() = default;

    void draw(Sprite *sprite);
    void clear();

    sf::Texture *texture = nullptr;
    sf::VertexArray vertexArray;
    std::size_t counter = 1;
};
