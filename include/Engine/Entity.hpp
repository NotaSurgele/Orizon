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
class Script;
class Velocity;
class BoxCollider;

class Entity {
    public:
        Entity();

        ~Entity();

        template <typename T, class... Args>
        T* addComponent(Args... args) {
            T *component = new T(this, args ...);
            _component_map.insert(std::pair<const char *, IComponent *>(SIGNATURE(T), component));
            component->setSignature(SIGNATURE(T));
            return component;
        }

        template <typename T, class... Args>
        T* addComponent(Entity *self, Args... args)
        {
            T *component = new T(self, args ...);

            _component_map.insert(std::pair<const char *, IComponent *>(SIGNATURE(T), component));
            component->setSignature(SIGNATURE(T));
            return component;
        }

        template <typename T = CustomComponents, class... Args>
        T* addCustomComponent(Args... args)
        {
            T *custom_component = new T(this, args ...);

            _custom_comp_map.insert(std::pair<const char *, CustomComponents *>(SIGNATURE(T), custom_component));
            custom_component->setSignature(SIGNATURE(T));
            return custom_component;
        }

        template <typename T>
        T* getComponent()
        {
            if (_component_map.size() <= 0 ) {
                return nullptr;
            }
            auto it = _component_map.find(SIGNATURE(T));
            if (it == _component_map.end())
                return nullptr;
            T* component = dynamic_cast<T *>(_component_map.find(SIGNATURE(T))->second);

            if (component == nullptr && DEBUG_MESSAGE) {
                std::cerr << "Component type " << SIGNATURE(T) <<
                    " does not exist in entity" << std::endl;
            }
            return component;
        }

        template <typename T>
        std::vector<T *> getComponents()
        {
            std::vector<T *> components;

            for (auto& it : _component_map) {
                std::string signature = SIGNATURE(T);

                if (signature.find(it.first) != std::string::npos) {
                    components.push_back(static_cast<T *>(it.second));
                }
            }
            return components;
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

        bool removeComponent(IComponent *c)
        {
            for (auto& it : _component_map) {
                if (it.second == c) {
                    _component_map.erase(it.first);
                    return true;
                }
            }
            return false;
        }

        template<typename T>
        bool removeComponent()
        {
            std::size_t removedValue = 0;

            for (auto& it : _component_map) {
                std::string componentSignature = it.first;
                if (componentSignature.find(SIGNATURE(T))
                    != std::string::npos) {
                    removedValue = _component_map.erase(it.first);
                    break;
                }
            }
            return removedValue > 0;
        }

        std::unordered_map<const char*, CustomComponents *> getCustomComponents()
        {
            return _custom_comp_map;
        }

        const std::size_t& get_id()
        {
            return getComponent<Id>()->get_id();
        }

        std::unordered_multimap<const char *, IComponent *> getComponents()
        {
            return _component_map;
        }

        void destroy();

    protected:
        std::unordered_multimap<const char *, IComponent *> _component_map;
        std::unordered_map<const char *, CustomComponents *> _custom_comp_map;
};

