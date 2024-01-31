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

void GameScene::create()
{
/*    addCustomComponentConstructor("CharacterController", [](Entity *e, nlohmann::json const& json) {
        auto speed = json["speed"];

        e->addCustomComponent<CharacterController>(speed);
    });*/
/*    sol::state lua;
    lua.open_libraries(sol::lib::base);

    sol::usertype<game> gameType = lua.new_usertype<game>("game",sol::constructors<game(int)>());
    gameType.set("isOdd", &game::isOdd);
    script();*/
    loadSceneFromFile("../assets/game.json");
    player = getEntity("player");
    auto canvas = player->addComponent<Canvas>();
    canvas->addText("Coucou", {0, 0}, 16);
    /*    toto = new Entity();
    System::pushEntity(toto);*/
/*
    player->getComponent<OrizonMusic>();
*/
/*
    player->addComponent<Script>("../assets/Scripting/Player.lua")->call();
*/
    auto c = sf::Color::Yellow;
    c.a = 40;
    //player->getComponent<Light>()->setColor(c);
    //player->getComponent<Sprite>()->setShadowIntensity(1);
/*    tiledMap = new TiledMap();
    tiledMap->load("../assets/map_test.tmj");
    player->addCustomComponent<CharacterController>(tiledMap);
    player->getComponent<BoxCollider>()->onCollision([&](BoxCollider *other) {
        other->entity()->destroy();
    });*/
}

void GameScene::update()
{
    if (Input::isActionKeyPressed("Refresh"))
    {
    }
    if (Input::isActionKeyPressed("Exit"))
        CLOSE();
    if (Input::isActionKeyPressed("Hide"))
        tiledMap->hide();

/*    auto animator = player->getComponent<Animator>();
    animator->playAnimation("idle", true);*/
/*
    script->call();
*/
}

void GameScene::destroy()
{
    player->destroy();
}
