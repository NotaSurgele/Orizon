#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "IComponent.hpp"

class IEntity {
    public:
        virtual IEntity& setPosition(sf::Vector2<float> const &position) = 0;
        virtual IEntity& setSize(sf::Vector2<float> const &size) = 0;
        virtual IEntity& setRotation(float rotation) = 0;

    private:

};
