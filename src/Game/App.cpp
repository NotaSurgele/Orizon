#include "Game/App.hpp"
#include "Engine/Components/Animator.hpp"

void App::start()
{
    loadInputFromFile(INPUT_FILE);

    game_scene.create();
    _sceneManager.addScene(game_scene);
}

void App::render()
{
    CoreClear(sf::Color::Black);
    Core::render();
}

void App::destroy()
{
    game_scene.destroy();
}
