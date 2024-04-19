#include "SpriteBatch.hpp"
#include <Math.hpp>

SpriteBatch::SpriteBatch()
{
}

void SpriteBatch::draw(Sprite *sprite)
{
    auto currentLength = counter * 4;

    if (currentLength + 4 > length) {
        length = currentLength + 4;
        vertexArray.resize(length);
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

    vertexArray.setPrimitiveType(sf::Quads);
    // Rotate the vertices
    vertexArray[counter * 4 + 0].position = rotate(sf::Vector2f(bounds.left, bounds.top));
    vertexArray[counter * 4 + 0].texCoords = sf::Vector2f(size.left, size.top);

    vertexArray[counter * 4 + 1].position = rotate(sf::Vector2f(bounds.left + bounds.width, bounds.top));
    vertexArray[counter * 4 + 1].texCoords = sf::Vector2f(size.left + size.width, size.top);

    vertexArray[counter * 4 + 2].position = rotate(sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height));
    vertexArray[counter * 4 + 2].texCoords = sf::Vector2f(size.left + size.width, size.top + size.height);

    vertexArray[counter * 4 + 3].position = rotate(sf::Vector2f(bounds.left, bounds.top + bounds.height));
    vertexArray[counter * 4 + 3].texCoords = sf::Vector2f(size.left, size.top + size.height);

    vertexArray[counter * 4 + 0].color = sprite->getColor();
    vertexArray[counter * 4 + 1].color = sprite->getColor();
    vertexArray[counter * 4 + 2].color = sprite->getColor();
    vertexArray[counter * 4 + 3].color = sprite->getColor();
    counter++;
}

void SpriteBatch::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (length <= 0) return;
    states.texture = &textureCpy;

    if (sprite->hasShader) {
        states.shader = **sprite->shader;
    }
    target.draw(&vertexArray[0], counter * 4, sf::Quads, states);
}

void SpriteBatch::clear()
{
    vertexArray.clear();
    counter = 0;
}

