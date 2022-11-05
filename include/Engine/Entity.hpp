#pragma once

#include "IEntity.hpp"
#include "Components/Transform2D.hpp"
#include "Components/IComponent.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <iostream>

#define TRANSFORM typeid(Transform2D).name()

class Entity : public IEntity {
    public:
        Entity() = default;
        ~Entity() = default;

        Entity& setPosition(sf::Vector2<float> const& position) override final;
        Entity& setSize(sf::Vector2<float> const& size) override final;
        Entity& setRotation(float rotation) override final;

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
            T* component = dynamic_cast<T*>(_component_map[SIGNATURE(T)]);

            if (component == nullptr) {
                std::cerr << "Component type " << SIGNATURE(T) <<
                    " does not exist in entity" << std::endl;
            }
            return component;
        }

        Transform2D *Transform();

        void destroy();

    private:

    protected:
        sf::Sprite _sprite;
        sf::Texture _texture;
        std::size_t _id;
        std::unordered_map<const char *, IComponent *> _component_map;
};
