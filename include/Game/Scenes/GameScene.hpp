#pragma once

#include "Engine/Scene/Scene.hpp"
#include "Game/Player.hpp"

class GameScene : public Scene {
public:
    void create() override final;
    void update() override final;
    void destroy() override final;

private:
    Player player = Player();
};
