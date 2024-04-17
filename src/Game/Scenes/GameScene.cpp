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
