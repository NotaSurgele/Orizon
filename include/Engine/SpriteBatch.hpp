#pragma once

#include <SFML/Graphics.hpp>
#include <Components/Sprite.hpp>

class SpriteBatch : public sf::Drawable {
public:
    SpriteBatch();
    ~SpriteBatch() override = default;

    void draw(Sprite *sprite);
    void clear();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:

public:
    const sf::Texture *texture = nullptr;
    sf::VertexArray vertexArray;
    sf::Texture textureCpy;
    std::size_t textureId = 0;
    std::size_t counter = 1;
    std::size_t length = 5;
};
