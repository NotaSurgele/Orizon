#include "Engine/System.hpp"
#include "Core.hpp"

    void System::force_system()
    {
       for (auto &it : _registry) {
           try {
                Entity *e = *it.second.get();
                auto velocity = e->getComponent<Velocity<float>>();
                auto transform = e->getComponent<Transform2D>();

                transform->position.x += velocity->getX() * Time::deltaTime;
                transform->position.y += velocity->getY() * Time::deltaTime;
           } catch (...) {
               std::cout << "Missing force field component" << std::endl;
           }
       }
    }

    void System::draw_system()
    {
        for (auto &it : _registry) {
            try {
                auto transform = (*it.second.get())->getComponent<Transform2D>();
                auto sprite = (*it.second.get())->getComponent<Sprite>();

                if (!transform)
                    sprite->setTransform(Transform2D::zero());
                else
                    sprite->setTransform(transform);
                DRAW(sprite);
            } catch(...) {
                std::cout << "RIP !" << std::endl;
            }
        }
    }

    void System::systems()
    {
        force_system();
        draw_system();
    }



