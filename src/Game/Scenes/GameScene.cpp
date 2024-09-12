#include "Game/Scenes/GameScene.hpp"
#include "Game/CustomComponents/CharacterController.hpp"
#include "Script.hpp"
#include "Core.hpp"

void GameScene::start()
{
    loadSceneFromFile("../assets/game.json");
    Scene::start();
}

void GameScene::update()
{
    if (Input::isActionKeyPressed("Exit"))
        CLOSE();
    Scene::update();
}

void GameScene::destroy()
{
    Scene::destroy();
}
