#include "Game/App.hpp"
#include "Engine/Components/Animator.hpp"

void App::start()
{
    loadInputFromFile(INPUT_FILE);
    game_scene.create();
}

void App::render()
{
    CoreClear(sf::Color::Black);
    game_scene.update();
}

void App::destroy()
{
    game_scene.destroy();
}
