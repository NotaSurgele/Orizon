#pragma once

#include "Entity.hpp"

class Player : public Entity {
public:
    Player() : Entity()
    {
    }

    void update()
    {
        std::cout << "hello world" << std::endl;
    }

    ~Player() = default;
};
