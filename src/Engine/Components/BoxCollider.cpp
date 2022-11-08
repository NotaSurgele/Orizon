#include "BoxCollider.hpp"

BoxCollider::BoxCollider(Entity *e,
                         const float& x, const float&y,
                         const float& width, const float& height) :
                         _related_entity(e),
                         _x(x),
                         _y(y),
                         _width(width),
                         _height(height)
{
}
