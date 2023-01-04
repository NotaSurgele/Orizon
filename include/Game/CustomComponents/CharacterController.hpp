#pragma once

#include "CustomComponents.hpp"
#include "Components/Velocity.hpp"
#include "Input.hpp"

class CharacterController : public CustomComponents {
    public:
        CharacterController(Entity *e) : _self(e) {}
        ~CharacterController() = default;

        void update() override
        {
            auto velocity = _self->getComponent<Velocity<float>>();

            if (Input::isActionPressed("MoveUp"))
                velocity->setY(-300);
            else if (Input::isActionPressed("MoveDown"))
                velocity->setY(300);
            else
                velocity->setY(0);

            if (Input::isActionPressed("MoveLeft"))
                velocity->setX(-300);
            else if (Input::isActionPressed("MoveRight"))
                velocity->setX(300);
            else
                velocity->setX(0);
        }

        void destroy() override final {};

    private:
        Entity *_self = nullptr;
};