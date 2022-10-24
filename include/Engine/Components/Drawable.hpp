#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Drawable : public sf::Drawable {
public:
    Drawable() = default;
    ~Drawable() = default;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};
