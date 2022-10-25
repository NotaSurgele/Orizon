#pragma once
#include <SFML/System.hpp>

class Time {
    public:
        static inline float deltaTime = 0.0f;

        Time();
        ~Time() = default;

        void update();

    private:
        sf::Clock _clock;
};
