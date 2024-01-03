#include "Game/App.hpp"
#include "Engine/Components/Animator.hpp"

void App::start()
{
    loadInputFromFile(INPUT_FILE);

    //game_scene.create();
    game_scene = new GameScene();
    _sceneManager.addScene(game_scene);
    _sceneManager.create();
}

void App::render()
{
    CoreClear(sf::Color::Black);
    _sceneManager.update();
}

void App::destroy()
{
    game_scene->destroy();
}
