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

private:
    std::size_t _counter = 1;
};
