#pragma once

#include "CustomComponents.hpp"
#include "Components/Velocity.hpp"
#include "Input.hpp"

class CharacterController : public CustomComponents {
    public:
        CharacterController(Entity *e, float const& speed) : _self(e), _speed(speed) {}
        ~CharacterController() = default;

        void update() override
        {
            auto velocity = _self->getComponent<Velocity<float>>();

            if (Input::isActionPressed("MoveUp"))
                velocity->setY(-_speed);
            else if (Input::isActionPressed("MoveDown"))
                velocity->setY(_speed);
            else
                velocity->setY(0);

            if (Input::isActionPressed("MoveLeft"))
                velocity->setX(-_speed);
            else if (Input::isActionPressed("MoveRight"))
                velocity->setX(_speed);
            else
                velocity->setX(0);
        }

        void destroy() override final {};

    private:
        const float _speed = 0.0f;
        Entity *_self = nullptr;
};
