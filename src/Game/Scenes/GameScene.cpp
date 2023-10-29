#include "Game/Scenes/GameScene.hpp"
#include "Engine/Components/Animator.hpp"
#include "Game/App.hpp"
#include "Engine/System.hpp"
#include "Game/CustomComponents/CharacterController.hpp"
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Light.hpp"

#include <random>
#include <math.h>

void GameScene::create()
{
    // _particles = Particles(10);
    // _particles.setShape(ParticleShape::SQUARE, 10);
    addCustomComponentConstructor("CharacterController", [](Entity *e, nlohmann::json const& json) {
        auto speed = json["speed"];

        e->addCustomComponent<CharacterController>(speed);
    });

    loadSceneFromFile("../assets/game.json");
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
    tiledMap->load("../assets/map_test.tmj");
}

void GameScene::update()
{
    if (Input::isActionPressed("Exit"))
        CLOSE();
    if (Input::isActionPressed("Refresh"))
    {
        CORE->loadInputFromFile(INPUT_FILE);
        tiledMap->load("../assets/map_test.tmj");
    }
    player->getComponent<Animator>()->playAnimation("idle", true);
}

void GameScene::loadTiledMap(const std::string &filePath)
{

}

void GameScene::destroy()
{
}
