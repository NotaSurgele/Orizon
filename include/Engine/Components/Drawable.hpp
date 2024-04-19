#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Shader.hpp"

class Drawable : public sf::Drawable {
public:
    Drawable() = default;
    ~Drawable() = default;


    void attachShader(Shader *s)
    {
        shader = s;
    }

    void dropShader()
    {
        hasShader = false;
        shader = nullptr;
    }

    Shader *shader = nullptr;
    bool hasShader = false;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};
