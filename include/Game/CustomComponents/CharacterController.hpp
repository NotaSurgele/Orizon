#pragma once

#include "CustomComponents.hpp"
#include "Components/Velocity.hpp"
#include "Input.hpp"
#include "TiledMap.hpp"

class CharacterController : public CustomComponents {
    public:
        CharacterController(Entity *e, TiledMap *tilemap) : _self(e), _tileMap(tilemap) {}
        ~CharacterController() = default;

        void update() override
        {
            if (!_render) {
                _tileMap->render();
                _render = !_render;
            }
           /* if (Input::isActionPressed("MoveUp"))
                velocity->setY(-_speed);
            else if (Input::isActionPressed("MoveDown"))
                velocity->setY(_speed);
            else
                velocity->setY(0);

            if (Input::isActionPressed("MoveLeft"))
                velocity->setX(-_speed);
            else if (Input::isActionKeyPressed("MoveRight"))
                velocity->setX(_speed);
            else
                velocity->setX(0);*/
/*           if (Input::isActionPressed("MoveUp"))
               transform->position.y -= 200 * Time::deltaTime;
           if (Input::isActionPressed("MoveDown"))
               transform->position.y += 200 * Time::deltaTime;
           if (Input::isActionPressed("MoveLeft"))
               transform->position.x -= 200 * Time::deltaTime;
           if (Input::isActionKeyPressed("MoveRight"))
               transform->position.x += 200 * Time::deltaTime;*/
        }

        void destroy() override final {}

    private:
        bool _render = false;
        TiledMap *_tileMap = nullptr;
        Entity *_self = nullptr;
};
