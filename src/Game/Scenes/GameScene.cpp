#include "Game/Scenes/GameScene.hpp"
#include "Engine/Components/Animator.hpp"
#include "Game/CustomComponents/CharacterController.hpp"
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"

#include <random>
#include <math.h>

void GameScene::create()
{
    addCustomComponentConstructor("CharacterController", [](Entity *e, nlohmann::json const& json) {
        auto speed = json["speed"];

        e->addCustomComponent<CharacterController>(speed);
    });

    loadSceneFromFile("../assets/game.json");
    tiledMap->load("../assets/map_test.tmj");
    player = getEntity("player");
    player->addComponent<Sound>();
    player->getComponent<OrizonMusic>();
    auto c = sf::Color::Yellow;
    c.a = 40;
    //player->getComponent<Light>()->setColor(c);
    //player->getComponent<Sprite>()->setShadowIntensity(1);

    player->getComponent<BoxCollider>()->onCollision([&](BoxCollider *other) {
        other->entity()->destroy();
    });
    //layer = new TileMap(0, 0, 3000, 3000, 16, 16);
}

void GameScene::update()
{
    if (Input::isActionPressed("Exit"))
        CLOSE();
    if (Input::isActionPressed("Hide"))
        tiledMap->hide();
    if (Input::isActionKeyDown("Render"))
    {
        tiledMap->render();
    }
    player->getComponent<Animator>()->playAnimation("idle", true);
}

void GameScene::destroy()
{
}
