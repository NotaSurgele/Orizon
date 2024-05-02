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
        hasShader = true;
        shader = s;
    }

    void dropShader()
    {
        hasShader = false;
        shader = nullptr;
    }

    Shader *shader = nullptr;
    bool hasShader = false;
    std::string name;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};
