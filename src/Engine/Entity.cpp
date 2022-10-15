#include "Engine/Entity.hpp"

Entity& Entity::setPosition(sf::Vector2<float> const &position)
{
    dynamic_cast<Transform2D *>(_component_map[TRANSFORM_IDX])->position = position;
    return *this;
}

Entity& Entity::setSize(sf::Vector2<float> const& size)
{
    dynamic_cast<Transform2D *>(_component_map[TRANSFORM_IDX])->size = size;
    return *this;
}

Entity& Entity::setRotation(float rotation)
{
    dynamic_cast<Transform2D *>(_component_map[TRANSFORM_IDX])->rotation = rotation;
    return *this;
}

Transform2D* Entity::Transform()
{
    return dynamic_cast<Transform2D *>(_component_map[TRANSFORM_IDX]);
}

