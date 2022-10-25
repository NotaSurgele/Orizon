#include "Game/App.hpp"

void App::start()
{
    loadInputFromFile("../assets/input.ini");
    R_ADD_RESSOURCE(sf::Texture, "player", "../assets/B_witch_attack.png");
    e.addComponent<Sprite>()->setTexture(R_GET_RESSOURCE(sf::Texture, "player"));
}

void App::render()
{
    if (Input::isActionPressed("MoveDown"))
        e.getComponent<Transform2D>()->position.y += 100.f * Time::deltaTime;
    if (Input::isActionPressed("MoveUp"))
        e.getComponent<Transform2D>()->position.y -= 100.f * Time::deltaTime;
    if (Input::isActionPressed("MoveLeft"))
        e.getComponent<Transform2D>()->position.x -= 100.f * Time::deltaTime;
    if (Input::isActionPressed("MoveRight"))
        e.getComponent<Transform2D>()->position.x += 100.f * Time::deltaTime;
    if (Input::isKeyDown("R"))
        loadInputFromFile("../assets/input.ini");
    if (Input::isKeyReleased("Space"))
        loadInputFromFile("../assets/input.ini");
    e.getComponent<Sprite>()->update();
    CoreDraw(e.getComponent<Sprite>());
}

void App::destroy()
{
}
