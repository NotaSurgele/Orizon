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
        _size.x = w;
        _size.y = h;
        if (ENGINE_MODE) {
            this->setSize(w * .5f, h * .5f);
/*            this->setViewport(sf::FloatRect(EDITOR_VIEW_SIZE_RATIO, 0,
                                            EDITOR_VIEW_SIZE_RATIO, EDITOR_VIEW_SIZE_RATIO));*/
        } else {
            this->setSize(w, h);
            //this->setViewport({x, y, 1.0f, 1.0f});
        }
    }

    void setViewPort(const sf::FloatRect& viewport)
    {
        if (ENGINE_MODE) {
            auto fixedViewport = sf::FloatRect { viewport.left * EDITOR_VIEW_SIZE_RATIO,
                                                 viewport.top * EDITOR_VIEW_SIZE_RATIO,
                                                 viewport.width,
                                                 viewport.height  };
            if (fixedViewport.left <= 0)
                fixedViewport.left = EDITOR_VIEW_SIZE_RATIO;
            return sf::View::setViewport(fixedViewport);
        }
        sf::View::setViewport(viewport);
    }

    bool& isFollowing() { return _follow; }

    void destroy() override final {}

    sf::FloatRect getViewBounds() const
    {
        if (ENGINE_MODE) {
            auto viewPos = getCenter() - (getSize() * .5f);

            return {viewPos, getSize() };
        }
        auto viewPos = getCenter() - (getSize());
        return { viewPos, getSize() };
    }

    void setViewBounds(const sf::FloatRect& bounds)
    {
        _size.x = bounds.width;
        _size.y = bounds.height;
        setCenter(bounds.left, bounds.top);
        setSize(bounds.width, bounds.height);
    }

    ~View() = default;

private:
    bool _follow = false;
    sf::Vector2f _size;
};