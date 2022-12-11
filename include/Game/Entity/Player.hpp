#pragma once

#include "Entity.hpp"

class Player : public Entity {
public:
    Player() : Entity()
    {
    }

    ~Player() = default;


    void update();
};
