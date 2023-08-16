#pragma once

#include "SFML/System.hpp"
#include <cmath>

class Math {
    Math() = delete;
    ~Math() = delete;

public:

    template <typename T>
    static inline sf::Vector2<T> direction(const sf::Vector2<T>& p1, const sf::Vector2<T>& p2)
    {
        sf::Vector2f dist = sf::Vector2f(p2.x - p1.x, p2.y - p1.y);
        float m = std::sqrt(dist.x * dist.x + dist.y * dist.y);

        return sf::Vector2<T>(dist.x / m, dist.y / m);
    }

    template <typename T>
    static inline sf::Vector2i direction2i(const sf::Vector2<T>& p1, const sf::Vector2<T>& p2)
    {
        sf::Vector2f dist = sf::Vector2f(p2.x - p1.x, p2.y - p1.y);
        float m = std::sqrt(dist.x * dist.x + dist.y * dist.y);

        return sf::Vector2i(dist.x / m, dist.y / m);
    }
};