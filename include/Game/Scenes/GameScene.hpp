#pragma once

#include "Engine/Scene/Scene.hpp"

class GameScene : public Scene {
public:
    void create() override final;
    void update() override final;
    void destroy() override final;

private:
    Entity e = Entity();
};
