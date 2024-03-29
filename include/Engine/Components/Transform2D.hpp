#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Entity.hpp"
#include "IComponent.hpp"

class Transform2D : public IComponent {
    public:
        float rotation = 0.0f;
        sf::Vector2<float> position = sf::Vector2<float>(0.0f, 0.0f);
        sf::Vector2<float> size = sf::Vector2<float>(1.0f, 1.0f);
        sf::Vector2<float> scale = sf::Vector2<float>(1.0f, 1.0f);

    public:

        Transform2D(Entity *e,
                    float x=0,
                    float y=0,
                    float width=1,
                    float height=1,
                    float scaleX=1,
                    float scaleY=1,
                    float rota=0)
        {
            position.x = x;
            position.y = y;
            size.x = width;
            size.y = height;
            scale.x = scaleX;
            scale.y = scaleY;
            rotation = rota;
        }

        Transform2D(
                    float x=0,
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

        Transform2D *operator=(Transform2D *other)
        {
            return other;
        }

        void destroy() override final { delete this; };

        static inline Transform2D* zero()
        {
            return new Transform2D();
        }

        ~Transform2D() = default;
};

