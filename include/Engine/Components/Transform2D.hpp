#pragma once

#include "IComponent.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Transform2D : public IComponent {
    private:
        static inline Transform2D *_zero;

    public:
        float rotation = 0.0f;
        sf::Vector2<float> position = sf::Vector2<float>(0.0f, 0.0f);
        sf::Vector2<float> size = sf::Vector2<float>(0.0f, 0.0f);

        static inline Transform2D* zero()
        {
            if (!_zero)
                _zero = new Transform2D();
            return _zero;
        }

    public:
        void destroy() override final { };

        Transform2D() = default;
        ~Transform2D() = default;
};

