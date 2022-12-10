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
           } catch(...) {
                std::cerr << "Entity does not have force component" << std::endl;
           }
        }
    }

    void System::draw_system()
    {
        for (auto &it : _registry) {
            auto entity = (*it.second.get());
            auto sprite = entity->getComponent<Sprite>();
            auto transform = entity->getComponent<Transform2D>();

            if (!sprite)
                continue;
            if (!entity->getComponent<Transform2D>())
                transform = Transform2D::zero();
            sprite->setTransform(transform);
            DRAW(sprite);
        }
    }

    void System::systems()
    {
        //force_system();
        draw_system();
    }



