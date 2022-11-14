#pragma once

#include "Components/Transform2D.hpp"
#include "Components/IComponent.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <iostream>
#include <memory>

#define TRANSFORM typeid(Transform2D).name()

using Component = std::shared_ptr<IComponent *>;

class Entity {
    public:
        Entity();
        ~Entity() = default;

        template <typename T, class... Args>
        T* addComponent(Args... args)
        {
            T *component = new T(args ...);

            _component_map.insert(std::pair<const char *, IComponent *>(SIGNATURE(T), component));
            return component;
        }

        template <typename T>
        T* getComponent()
        {
            return dynamic_cast<T*>(_component_map[SIGNATURE(T)]);
        }

        const std::size_t& get_id()
        {
            return id;
        }

        void destroy();

    protected:
        std::size_t id = 0;
        const Entity *_e = nullptr;
        std::unordered_map<const char *, IComponent *> _component_map;
};
