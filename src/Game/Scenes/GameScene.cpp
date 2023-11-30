#include "Game/Scenes/GameScene.hpp"
#include "Engine/Components/Animator.hpp"
#include "Game/CustomComponents/CharacterController.hpp"
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Script.hpp"

#include <random>
#include <math.h>

void GameScene::create()
{
    addCustomComponentConstructor("CharacterController", [](Entity *e, nlohmann::json const& json) {
        auto speed = json["speed"];

        e->addCustomComponent<CharacterController>(speed);
    });

/*    sol::state lua;
    lua.open_libraries(sol::lib::base);

    sol::usertype<game> gameType = lua.new_usertype<game>("game",sol::constructors<game(int)>());
    gameType.set("isOdd", &game::isOdd);
    script();*/
    loadSceneFromFile("../assets/game.json");
    //tiledMap->load("../assets/map_test.tmj");
    player = getEntity("player");
    player->addComponent<Sound>();
/*
    player->getComponent<OrizonMusic>();
*/
    Entity *toto = new Entity();
    toto->addComponent<Script>("../assets/Scripting/helloWorld.lua")->call();
/*
    player->addComponent<Script>("../assets/Scripting/helloWorld.lua")->call();
*/
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
    if (Input::isActionKeyDown("Refresh")) {
        destroy();
        create();
    }
    if (Input::isActionPressed("Exit"))
        CLOSE();
    if (Input::isActionPressed("Hide"))
        tiledMap->hide();
    if (Input::isActionKeyDown("Render"))
    {
        tiledMap->render();
    }
    /*if (player) {
        std::cout << "goodbye" << std::endl;
        auto animator =    player->getComponent<Animator>();
        if (animator) {
            animator->playAnimation("idle", true);
        }
    }*/
}

void GameScene::destroy()
{
    player->destroy();
}
