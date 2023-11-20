#pragma once
#include <SFML/System.hpp>

class Time {
    public:
        static inline float deltaTime = 0.0f;

        Time();
        ~Time() = default;

        sf::Time start();
        sf::Clock& getClock();
        sf::Time end();
    private:
        sf::Clock _clock;
};
