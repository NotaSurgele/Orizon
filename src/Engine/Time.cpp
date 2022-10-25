#include "Engine/Time.hpp"

Time::Time() : _clock()
{
}

void Time::update()
{
    sf::Time dt = _clock.restart();

    deltaTime = dt.asSeconds();
}
