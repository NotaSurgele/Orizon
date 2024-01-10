#include <iostream>
#include "Engine/Entity.hpp"
#include "Engine/System.hpp"

int i = 0;

Entity::Entity()
{
    System::addEntity(this);
}

void Entity::__destroyComponents()
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
