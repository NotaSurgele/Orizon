//
// Created by alexi on 11/04/2024.
//

#include "SpriteBatch.hpp"

SpriteBatch::SpriteBatch()
{
    vertexArray = sf::VertexArray(sf::Quads, 4 * 4);
}

void SpriteBatch::draw(Sprite *sprite)
{
    auto length = vertexArray.getVertexCount();

    if (length < _counter * 4) {
        vertexArray.resize((_counter * 4) + 4);
    }

    auto bounds = sprite->getGlobalBounds();
    auto size = bounds.getSize();
    auto position = bounds.getPosition();

    texture = (sf::Texture *)sprite->getTexture();

    // Set vertex data
    vertexArray[_counter].position = sf::Vector2f(bounds.left, bounds.top);
    vertexArray[_counter].texCoords = sf::Vector2f(0, 0);
    vertexArray[_counter].color = sprite->getColor();

    vertexArray[_counter + 1].position = sf::Vector2f(bounds.left + bounds.width, bounds.top);
    vertexArray[_counter + 1].texCoords = sf::Vector2f(bounds.width, 0);
    vertexArray[_counter + 1].color = sprite->getColor();

    vertexArray[_counter + 2].position = sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height);
    vertexArray[_counter + 2].texCoords = sf::Vector2f(bounds.width, bounds.height);
    vertexArray[_counter + 2].color = sprite->getColor();

    vertexArray[_counter + 3].position = sf::Vector2f(bounds.width, bounds.top + bounds.height);
    vertexArray[_counter + 3].texCoords = sf::Vector2f(0, bounds.height);
    vertexArray[_counter + 3].color = sprite->getColor();
    _counter++;
}

void SpriteBatch::clear()
{
    vertexArray.clear();
    _counter = 1;
}

