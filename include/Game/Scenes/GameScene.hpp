#pragma once

#include <Script.hpp>

#include "Engine/Scene/Scene.hpp"
#include "Game/Entity/Player.hpp"
#include "external/OpenSimplexNoise.hpp"
#include "Engine/Components/Particles.hpp"
#include "Engine/Collision/Layer/TileMap.hpp"
#include "Engine/RayCaster.hpp"
#include"OrizonMusic.hpp"
#include "TiledMap.hpp"

class GameScene : public Script {
public:
    GameScene(const std::string& path) : Script(path) {}

    void start() final;
    void update() final;
    void destroy() final;

private:

private:
    Entity *player = nullptr;
    TileMap *layer = nullptr;
    std::list<Entity *> _blocks;
    std::vector<std::vector<int>> _heightMap;
    bool load = false;
    Particles _particles;
    sf::RectangleShape wall = sf::RectangleShape();
    sf::RectangleShape wall2 = sf::RectangleShape();
    std::vector<RayCaster> _rayCaster;
    OrizonMusic *_music = nullptr;
    TiledMap *tiledMap = new TiledMap();
};
