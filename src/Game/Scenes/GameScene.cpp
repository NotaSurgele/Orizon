#include "Game/Scenes/GameScene.hpp"
#include "Engine/Components/Animator.hpp"
#include "Game/App.hpp"

void GameScene::create()
{
    R_ADD_RESSOURCE(sf::Texture, "hobbit", "../assets/Hobbit/pngs/hobbit_idle_spritesheet.png");
    e.addComponent<Sprite>(&e, R_GET_RESSOURCE(sf::Texture, "hobbit"), 10, 10);
    e.addComponent<Animator>(&e, 4, AnimatorRect{0, 0, 64, 64}, .2f);
    e.addComponent<Transform2D>();
}

void GameScene::update()
{
    if (Input::isActionPressed("MoveDown"))
        e.getComponent<Transform2D>()->position.y += 100.f * Time::deltaTime;
    if (Input::isActionPressed("MoveUp"))
        e.getComponent<Transform2D>()->position.y -= 100.f * Time::deltaTime;
    if (Input::isActionPressed("MoveLeft"))
        e.getComponent<Transform2D>()->position.x -= 100.f * Time::deltaTime;
    if (Input::isActionPressed("MoveRight"))
        e.getComponent<Transform2D>()->position.x += 100.f * Time::deltaTime;
    if (Input::isActionPressed("Exit"))
        CORE->CoreClose();
    if (Input::isActionPressed("Refresh"))
        CORE->loadInputFromFile(INPUT_FILE);
    e.getComponent<Animator>()->playAnimation(true);
    e.getComponent<Sprite>()->update();
    CORE->CoreDraw(e.getComponent<Sprite>());
}

void GameScene::destroy()
{
    e.destroy();
}
