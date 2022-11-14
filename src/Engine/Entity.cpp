#include "Engine/System.hpp"
#include <iostream>

Entity::Entity() : _e(this),
                   id(System::get_id())
{
    System::addEntity(this);
}

void Entity::destroy()
{
    for (auto &it : _component_map) {
        it.second->destroy();
    }
}

