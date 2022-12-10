#include <iostream>
#include "Engine/Entity.hpp"
#include "Engine/System.hpp"

Entity::Entity()
{
    System::addEntity(this);
}

void Entity::destroy()
{
    for (auto &it : _component_map) {
        it.second->destroy();
    }
}
