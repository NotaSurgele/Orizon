#pragma once

#include "IComponent.hpp"
#include "Entity.hpp"
#include "config.hpp"

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
        if (ENGINE_MODE) {
            this->setSize(w * .5f, h * .5f);
            this->setViewport(sf::FloatRect(.25, 0, .5f, .5f));
        }
    }

    const bool& isFollowing() { return _follow; }

    void destroy() override final {}

    ~View() = default;

private:
    const bool _follow = false;
};