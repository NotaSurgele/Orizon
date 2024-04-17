#include "Game/Scenes/GameScene.hpp"
#include "Engine/Components/Animator.hpp"
#include "Game/CustomComponents/CharacterController.hpp"
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Script.hpp"
#include "Utils.hpp"
#include "sol/sol.hpp"
#include "Canvas.hpp"

#include <random>
#include <math.h>

std::vector<Sprite *> array;

void GameScene::start()
{
    loadSceneFromFile("../assets/game.json");
    Script::start();
    float x = 400;
    for (std::size_t i = 0; i < 10; i++) {
        auto texture = R_GET_RESSOURCE(sf::Texture, "hobbit");
        auto s = new Sprite(texture);

        s->setPosition(x, 500);
        array.push_back(s);
        x += 0.1f;
    }
}

void GameScene::update()
{
    for (auto& s : array) {
        DRAW_BATCH(s);
    }
    if (Input::isActionKeyPressed("Refresh"))
    {
    }
    if (Input::isActionKeyPressed("Exit"))
        CLOSE();
    if (Input::isActionKeyPressed("Hide"))
        tiledMap->hide();
    Script::update();

/*    auto animator = player->getComponent<Animator>();
    animator->playAnimation("idle", true);*/
/*
    script->call();
*/
}

void GameScene::destroy()
{
    Script::destroy();
}
