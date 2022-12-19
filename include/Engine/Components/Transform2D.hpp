#pragma once

#include "IComponent.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Transform2D : public IComponent {
    public:
        float rotation = 0.0f;
        sf::Vector2<float> position = sf::Vector2<float>(0.0f, 0.0f);
        sf::Vector2<float> size = sf::Vector2<float>(1.0f, 1.0f);

    public:
        Transform2D(float x=0,
                    float y=0,
                    float width=1,
                    float height=1,
                    float rota=0)
        {
            position.x = x;
            position.y = y;
            size.x = width;
            size.y = height;
            rotation = rota;
        }

        void destroy() override final { };

        static inline Transform2D* zero()
        {
            return new Transform2D();
        }

        ~Transform2D() = default;
};

