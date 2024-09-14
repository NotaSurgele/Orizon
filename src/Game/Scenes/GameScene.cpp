#include "Game/Scenes/GameScene.hpp"
#include "Game/CustomComponents/CharacterController.hpp"
#include "Script.hpp"
#include "Core.hpp"

void GameScene::start()
{
    loadSceneFromFile("../assets/game.json");
    Script::start();
}

void GameScene::update()
{
    if (Input::isActionKeyPressed("Exit"))
        CLOSE();
    Script::update();
}

void GameScene::destroy()
{
    Script::destroy();
}