//
// Created by alexi on 11/04/2024.
//

#include "SpriteBatch.hpp"
#include <Math.hpp>

SpriteBatch::SpriteBatch()
{
    vertexArray.resize(5);
}

void SpriteBatch::draw(Sprite *sprite)
{
    auto length = counter * 4;

    if (length + 4 > vertexArray.size()) {
        std::cout << "Resizing" << std::endl;
        vertexArray.resize(vertexArray.size() + 4);
    }

    auto bounds = sprite->getGlobalBounds();
    auto size = sprite->getTextureRect();

    // Set vertex data
    float rotation = sprite->getRotation() * 3.14159f / 180.f;

    // Get the origin of rotation (center of the sprite)
    sf::Vector2f origin = sf::Vector2f(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);

    // Define a lambda function to rotate a point around the origin
    auto rotate = [&](sf::Vector2f point) {
        point -= origin;
        return sf::Vector2f(
                point.x * cos(rotation) - point.y * sin(rotation),
                point.x * sin(rotation) + point.y * cos(rotation)
        ) + origin;
    };

    // Rotate the vertices
    vertexArray[counter].position = rotate(sf::Vector2f(bounds.left, bounds.top));
    vertexArray[counter + 1].position = rotate(sf::Vector2f(bounds.left + bounds.width, bounds.top));
    vertexArray[counter + 2].position = rotate(sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height));
    vertexArray[counter + 3].position = rotate(sf::Vector2f(bounds.left, bounds.top + bounds.height));

    vertexArray[counter].texCoords = sf::Vector2f(size.left, size.top);
    vertexArray[counter + 1].texCoords = sf::Vector2f(size.left + size.width, size.top);
    vertexArray[counter + 2].texCoords = sf::Vector2f(size.left + size.width, size.top + size.height);
    vertexArray[counter + 3].texCoords = sf::Vector2f(size.left, size.top + size.height);

    vertexArray[counter].color = sprite->getColor();
    vertexArray[counter + 1].color = sprite->getColor();
    vertexArray[counter + 2].color = sprite->getColor();
    vertexArray[counter + 3].color = sprite->getColor();
    std::cout << "length " << length << " counter " << counter << " array size " << vertexArray.size() / 4 << std::endl;
    counter++;
}

void SpriteBatch::clear()
{
    vertexArray.clear();
    counter = 0;
}

