#include "Engine/Time.hpp"

Time::Time() : _clock()
{
}

sf::Clock& Time::getClock()
{
    return _clock;
}

void Time::update()
{
    sf::Time dt = _clock.restart();

    deltaTime = dt.asSeconds();
    if (deltaTime > 2.0f)
        deltaTime = 0.1f;
}
