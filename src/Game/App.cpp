#include "Game/App.hpp"
#include "Script.hpp"
#include "Engine/Components/Animator.hpp"

void App::start()
{
    loadInputFromFile(INPUT_FILE);

    //game_scene.create();
    _sceneManager.addScene(new GameScene("../assets/Scripting/Game.lua"));
    _sceneManager.create();
}

void App::render()
{
    CoreClear(sf::Color::Black);
    Core::render();
}

void App::destroy()
{
    game_scene->destroy();
}
