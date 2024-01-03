#pragma once

#include "IComponent.hpp"
#include "Entity.hpp"
#include "config.hpp"

#define EDITOR_VIEW_SIZE_RATIO .35f

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
            _size.x = w;
            _size.y = h;
            this->setSize(w * .5f, h * .5f);
            this->setViewport(sf::FloatRect(EDITOR_VIEW_SIZE_RATIO, 0,
                                            EDITOR_VIEW_SIZE_RATIO, EDITOR_VIEW_SIZE_RATIO));
        }
    }

    const bool& isFollowing() { return _follow; }

    void destroy() override final {}

    sf::FloatRect getViewBounds() const
    {
        auto viewPos = getCenter() - (_size / 2.0f);

        return {viewPos, _size };
    }

    ~View() = default;

private:
    const bool _follow = false;
    sf::Vector2f _size;
};