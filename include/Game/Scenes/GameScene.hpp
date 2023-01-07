#pragma once

#include "Engine/Scene/Scene.hpp"
#include "Game/Entity/Player.hpp"

class GameScene : public Scene {
public:
    void create() override final;
    void update() override final;
    void destroy() override final;

private:
    Entity *player = nullptr;
    std::vector<Entity *> _blocks;
    std::vector<std::vector<int>> _heightMap;
    bool load = false;
};
