#pragma once

#include "Engine/Scene/Scene.hpp"
#include "Game/Entity/Player.hpp"
#include "external/OpenSimplexNoise.hpp"
#include "Engine/Particle/Particles.hpp"
#include "Engine/Collision/Layer/CollidingLayer.hpp"
#include "Engine/Raytracer.hpp"

class GameScene : public Scene {
public:
    void create() override final;
    void update() override final;
    void destroy() override final;

private:
    Entity *player = nullptr;
    CollidingLayer *layer = nullptr;
    std::vector<Entity *> _blocks;
    std::vector<std::vector<int>> _heightMap;
    bool load = false;
    Particles _particles;
    sf::RenderTexture test;
    sf::RectangleShape wall = sf::RectangleShape();
    sf::RectangleShape wall2 = sf::RectangleShape();
    std::vector<RayTracer> _rayCaster;
};
