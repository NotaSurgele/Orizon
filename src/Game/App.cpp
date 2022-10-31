#include "Game/App.hpp"
#include "Engine/Components/Animator.hpp"

void App::start()
{
    loadInputFromFile(INPUT_FILE);
    R_ADD_RESSOURCE(sf::Texture, "hobbit", "../assets/Hobbit/pngs/hobbit_idle_spritesheet.png");
    e.addComponent<Sprite>(&e, R_GET_RESSOURCE(sf::Texture, "hobbit"), 10, 10);
    e.addComponent<Animator>(&e, 4, AnimatorRect{0, 0, 64, 64}, .2f);
    e.addComponent<Transform2D>();
}

void App::render()
{
    CoreClear(sf::Color::Black);
    if (Input::isActionPressed("MoveDown"))
        e.getComponent<Transform2D>()->position.y += 100.f * Time::deltaTime;
    if (Input::isActionPressed("MoveUp"))
        e.getComponent<Transform2D>()->position.y -= 100.f * Time::deltaTime;
    if (Input::isActionPressed("MoveLeft"))
        e.getComponent<Transform2D>()->position.x -= 100.f * Time::deltaTime;
    if (Input::isActionPressed("MoveRight"))
        e.getComponent<Transform2D>()->position.x += 100.f * Time::deltaTime;
    if (Input::isActionPressed("Exit"))
        CoreClose();
    if (Input::isActionPressed("Refresh"))
        loadInputFromFile(INPUT_FILE);
    e.getComponent<Animator>()->playAnimation(true);
    e.getComponent<Sprite>()->update();
    CoreDraw(e.getComponent<Sprite>());
}

void App::destroy()
{
    e.destroy();
}
