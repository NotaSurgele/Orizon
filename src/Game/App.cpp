#include "Game/App.hpp"

void App::start()
{
    R_ADD_RESSOURCE(sf::Texture, "player", "../assets/B_witch_attack.png");
    e.addComponent<Sprite>()->setTexture(R_GET_RESSOURCE(sf::Texture, "player"));
}

void App::render()
{
    if (Input::isActionPressed("MoveDown"))
        e.getComponent<Transform2D>()->position.y += 100.f * Core::getDeltaTime();
    if (Input::isActionPressed("MoveUp"))
        e.getComponent<Transform2D>()->position.y -= 100.f * Core::getDeltaTime();
    if (Input::isActionPressed("MoveLeft"))
        e.getComponent<Transform2D>()->position.x -= 100.f * Core::getDeltaTime();
    if (Input::isActionPressed("MoveRight"))
        e.getComponent<Transform2D>()->position.x += 100.f * Core::getDeltaTime();
    e.getComponent<Sprite>()->update();
    CoreDraw(e.getComponent<Sprite>());
}

void App::destroy()
{
}