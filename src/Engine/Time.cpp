#include "Engine/Time.hpp"

Time::Time() : _clock()
{
}

sf::Clock& Time::getClock()
{
    return _clock;
}

sf::Time Time::start()
{
    _clock.restart();
}

sf::Time Time::end()
{
    sf::Time dt = _clock.getElapsedTime();

    deltaTime = dt.asSeconds();
    if (deltaTime > 2.0f)
        deltaTime = .1f;
}
