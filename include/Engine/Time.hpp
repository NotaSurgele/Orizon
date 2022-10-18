#pragma once
#include <SFML/System.hpp>

class Time {
    public:
        Time();
        ~Time() = default;

        void update();
        static float getDeltaTime();

    private:
        sf::Clock _clock;
        static inline float deltaTime = 0.0f;
};