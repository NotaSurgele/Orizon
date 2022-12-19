#pragma once

#include "Engine/Scene/Scene.hpp"
#include "Game/Entity/Player.hpp"

class GameScene : public Scene {
public:
    void create() override final;
    void update() override final;
    void destroy() override final;

private:
    Player player = Player();
    Entity player2 = Entity();
    Entity dirt = Entity();
};
