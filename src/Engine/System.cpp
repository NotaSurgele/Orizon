#include "Engine/System.hpp"
#include "Core.hpp"

    void System::force_system()
    {
       for (auto &it : _registry) {
           Entity *e = *it.second.get();
           auto velocity = e->getComponent<Velocity<float>>();
           auto transform = e->getComponent<Transform2D>();

           if (velocity != nullptr && transform != nullptr) {
               transform->position.x += velocity->getX() * Time::deltaTime;
               transform->position.y += velocity->getY() * Time::deltaTime;
           }
       }
    }

    void System::draw_system()
    {
        for (auto &it : _registry) {
            Entity *e = *it.second.get();
            auto transform = e->getComponent<Transform2D>();
            auto sprite = e->getComponent<Sprite>();

            if (sprite == nullptr)
                continue;
            if (transform != nullptr) {
                sprite->getSprite().setPosition(transform->position);
                sprite->getSprite().setRotation(transform->rotation);
            } else
                sprite->getSprite().setPosition(0, 0);
            DRAW(sprite);
        }
    }

    void System::systems()
    {
        force_system();
        draw_system();
    }



