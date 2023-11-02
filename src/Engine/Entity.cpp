#include <iostream>
#include "Engine/Entity.hpp"
#include "Engine/System.hpp"

Entity::Entity()
{
    System::addEntity(this);
}

Entity::~Entity()
{
    for (auto &it : _component_map) {
        if (it.second) {
            it.second->destroy();
        }
    }
    _component_map.clear();
}

void Entity::destroy()
{
    System::RemoveEntity(this);
}
