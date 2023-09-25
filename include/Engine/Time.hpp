#pragma once
#include <SFML/System.hpp>

class Time {
    public:
        static inline float deltaTime = 0.0f;

        Time();
        ~Time() = default;

        sf::Clock& getClock();
        void update();

    private:
        sf::Clock _clock;
};
