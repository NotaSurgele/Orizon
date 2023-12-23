#include <iostream>
#include "Engine/Entity.hpp"
#include "Engine/System.hpp"

int i = 0;

Entity::Entity()
{
    std::cout << i++ << std::endl;
    System::addEntity(this);
}

Entity::~Entity()
{
    std::cout << "Hello World" << std::endl;
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
