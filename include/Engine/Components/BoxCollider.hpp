#pragma once

#include "IComponent.hpp"
#include "Entity.hpp"

class BoxCollider : public IComponent {
public:
    BoxCollider(
        Entity *e,
        const float& x, const float& y,
        const float& width, const float& height);
    ~BoxCollider() = default;

    void destroy() override final {}

    Entity *getRelatedEntity();

private:
    Entity *_related_entity = nullptr;
    float _x = 0.0f;
    float _y = 0.0f;
    float _width = 0.0f;
    float _height = 0.0f;
};

