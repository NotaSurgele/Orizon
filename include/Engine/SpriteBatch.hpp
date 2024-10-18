#pragma once

#include <SFML/Graphics.hpp>
#include <Components/Sprite.hpp>

class SpriteBatch : public sf::Drawable {
public:
    SpriteBatch(const int& z=0);
    ~SpriteBatch() override = default;

    void draw(Sprite *sprite);
    void draw(const sf::Shape& shape);
    void draw(const sf::Shape& shape, const sf::BlendMode& mode);

    void draw(Sprite *sprite, const sf::BlendMode& mode);

    void clear();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
#ifdef ENGINE_GUI
    sf::RenderTexture _renderTexture;
#endif
    sf::BlendMode _blendMode;
    const sf::Texture *texture = nullptr;
    sf::VertexArray vertexArray;
    sf::Texture textureCpy;
    std::size_t textureId = 0;
    std::size_t counter = 1;
    std::size_t length = 5;
    int zIndex = 0;
    Sprite *savedSprite = nullptr;
};

