#include "Engine/System.hpp"
#include "Core.hpp"

    void System::force_system()
    {
       for (auto &it : _registry) {
           Entity *e = *it.second.get();
           auto velocity = e->getComponent<Velocity<float>>();
           auto transform = e->getComponent<Transform2D>();

           if (!velocity || !transform)
               continue;
           transform->position.x += velocity->getX() * Time::deltaTime;
           transform->position.y += velocity->getY() * Time::deltaTime;
       }
    }

    void System::draw_system()
    {
        for (auto &it : _registry) {
            auto transform = (*it.second.get())->getComponent<Transform2D>();
            auto sprite = (*it.second.get())->getComponent<Sprite>();

            if (!sprite)
                continue;
            if (!transform) {
                sprite->setTransform(Transform2D::zero());
            } else
                sprite->setTransform(transform);
            DRAW(sprite);
        }
    }

    void System::systems()
    {
        force_system();
        draw_system();
    }



