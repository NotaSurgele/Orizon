#pragma once

#include "Components/IComponent.hpp"
#include "Components/Id.hpp"
#include "Components/CustomComponents.hpp"
#include "config.hpp"
#include <SFML/System.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <iostream>
#include <memory>

class Sprite;
class Animator;
class Transform2D;

template <typename T>
class Velocity;
class BoxCollider;

class Entity {
    public:
        Entity();

        virtual ~Entity() = default;

        template <typename T, class... Args>
        T* addComponent(Args... args)
        {
            T *component = new T(this, args ...);

            if (_component_map.contains(SIGNATURE(T))) {
                T *c = dynamic_cast<T *>(_component_map[SIGNATURE(T)]);
                delete component;
                return c;
            }
            _component_map.insert(std::pair<const char *, IComponent *>(SIGNATURE(T), component));
            return component;
        }

        template <typename T = CustomComponents, class... Args>
        T* addCustomComponent(Args... args)
        {
            T *custom_component = new T(this, args ...);

            if (_custom_comp_map.contains(SIGNATURE(T))) {
                T *c = dynamic_cast<T *>(_custom_comp_map[SIGNATURE(T)]);
                delete custom_component;
                return c;
            }
            _custom_comp_map.insert(std::pair<const char *, CustomComponents *>(SIGNATURE(T), custom_component));
            return custom_component;
        }

        template <typename T>
        T* getComponent()
        {
            if (_component_map.size() <= 0 ) {
                return nullptr;
            }
            T* component = dynamic_cast<T *>(_component_map[SIGNATURE(T)]);

            if (component == nullptr && DEBUG_MESSAGE) {
                std::cerr << "Component type " << SIGNATURE(T) <<
                    " does not exist in entity" << std::endl;
            }
            return component;
        }

        template <typename First, typename... Others>
        typename std::enable_if<sizeof...(Others) != 0, bool>::type
        hasComponents()
        {
            return this->getComponent<First>() != nullptr && hasComponents<Others ...>();
        }

        template <typename Last>
        bool hasComponents()
        {
            return this->getComponent<Last>() != nullptr;
        }

        std::unordered_map<const char*, CustomComponents *> getCustomComponents()
        {
            return _custom_comp_map;
        }

        const std::size_t& get_id()
        {
            return getComponent<Id>()->get_id();
        }

        void destroy();

    protected:
        std::unordered_map<const char *, IComponent *> _component_map;
        std::unordered_map<const char *, CustomComponents *> _custom_comp_map;
};

