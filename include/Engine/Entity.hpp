#pragma once

#include "Components/Transform2D.hpp"
#include "Components/IComponent.hpp"
#include "Components/Id.hpp"
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

        virtual ~Entity() = default;

        template <typename T, class... Args>
        T* addComponent(Args... args)
        {
            T *component = new T(this, args ...);

            if (_component_map.contains(SIGNATURE(T))) {
                T *c = dynamic_cast<T*>(_component_map[SIGNATURE(T)]);
                delete component;
                return c;
            }
            _component_map.insert(std::pair<const char *, IComponent *>(SIGNATURE(T), component));
            return component;
        }

        template <typename T>
        T* getComponent()
        {
            T* component = dynamic_cast<T *>(_component_map[SIGNATURE(T)]);

            if (component == nullptr) {
                std::cerr << "Component type " << SIGNATURE(T) <<
                    " does not exist in entity" << std::endl;
            }
            return component;
        }

        const std::size_t& get_id()
        {
            return getComponent<Id>()->get_id();
        }

        void destroy();

    protected:
        const Entity *_e = nullptr;
        std::unordered_map<const char *, IComponent *> _component_map;
};
