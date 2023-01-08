#pragma once

#include "IComponent.hpp"
#include "Entity.hpp"

class View : public IComponent, public sf::View {
public:
    View(Entity *e,
        const float& x,
        const float& y,
        const float& w,
        const float& h,
        const bool& follow=false)
        : sf::View(sf::Vector2f(x, y), sf::Vector2f(w, h)),
        _follow(follow)
    {
    }

    const bool& isFollowing() { return _follow; }

    void destroy() override final {}

    ~View() = default;

private:
    const bool _follow = false;
};