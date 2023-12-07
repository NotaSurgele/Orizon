#pragma once

#include "Engine/Scene/Scene.hpp"
#include "Game/Entity/Player.hpp"
#include "external/OpenSimplexNoise.hpp"
#include "Engine/Particle/Particles.hpp"
#include "Engine/Collision/Layer/TileMap.hpp"
#include "Engine/RayCaster.hpp"
#include"OrizonMusic.hpp"
#include "TiledMap.hpp"

class GameScene : public Scene {
public:
    void create() override final;
    void update() override final;
    void destroy() override final;

private:

private:
    Entity *player = nullptr;
    TileMap *layer = nullptr;
    std::vector<Entity *> _blocks;
    std::vector<std::vector<int>> _heightMap;
    bool load = false;
    Particles _particles;
    sf::RenderTexture test;
    sf::RectangleShape wall = sf::RectangleShape();
    sf::RectangleShape wall2 = sf::RectangleShape();
    std::vector<RayCaster> _rayCaster;
    OrizonMusic *_music = nullptr;
    TiledMap *tiledMap = new TiledMap();
    Entity *toto = nullptr;
};
