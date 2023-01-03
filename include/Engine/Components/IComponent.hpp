#pragma once

#include <map>
#include <functional>
#include <string>
#include <iostream>

#define SIGNATURE(type) \
    typeid(type).name()

class Entity;

class IComponent {
  public:
    IComponent() = default;
    ~IComponent() = default;

    virtual void destroy() = 0;
};

class ComponentFactory {
    public:

        template <class... Args>
        static void create_component(std::string const &name, Entity *e, Args... args);

        template <class... Args>
        static void create_transform(Entity *e, Args... args);
};