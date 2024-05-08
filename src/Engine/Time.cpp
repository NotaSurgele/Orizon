#include "Engine/Time.hpp"
#include <iostream>

Time::Time() : _clock()
{
}

sf::Clock& Time::getClock()
{
    return _clock;
}

sf::Time Time::start()
{
    return _clock.restart();
}

sf::Time Time::end()
{
    sf::Time dt = _clock.getElapsedTime();

    deltaTime = dt.asSeconds();
    if (deltaTime > 2.0f) {
        std::cout << "ahaha" << std::endl;
        deltaTime = .1f;
    }
    return dt;
}
