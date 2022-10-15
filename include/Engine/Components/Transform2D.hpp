#pragma once

#include "IComponent.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Transform2D : public IComponent {
    public:
        float rotation = 0.0f;
        sf::Vector2<float> position = sf::Vector2<float>(0.0f, 0.0f);
        sf::Vector2<float> size = sf::Vector2<float>(0.0f, 0.0f);

        void update() override final {};

        Transform2D() = default;
        ~Transform2D() = default;
};