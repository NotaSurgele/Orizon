#include "Game/Scenes/GameScene.hpp"
#include "Engine/Components/Animator.hpp"
#include "Game/App.hpp"
#include "Engine/System.hpp"
#include "Components/Velocity.hpp"

void GameScene::create()
{
    R_ADD_RESSOURCE(sf::Texture, "hobbit", "../assets/Hobbit/pngs/hobbit_idle_spritesheet.png");
    e.addComponent<Sprite>(&e, R_GET_RESSOURCE(sf::Texture, "hobbit"), 10, 10);
    e.addComponent<Animator>(&e)->newAnimation(4, AnimatorRect{0, 0, 64, 64}, .2f, "idle");
    e.addComponent<Transform2D>();
    e.addComponent<Velocity<float>>();
    System::addEntity<Entity>(&e);
}

void GameScene::update()
{
    auto velocity = e.getComponent<Velocity<float>>();
    if (Input::isActionPressed("MoveDown"))
        velocity->setY(-20.0f);
    if (Input::isActionPressed("MoveUp"))
        velocity->setY(20.0f);
    if (Input::isActionPressed("MoveLeft"))
        velocity->setX(-20.0f);
    if (Input::isActionPressed("MoveRight"))
        velocity->setX(20.0f);
    if (Input::isActionPressed("Exit"))
        CLOSE();
    if (Input::isActionPressed("Refresh"))
        CORE->loadInputFromFile(INPUT_FILE);
    e.getComponent<Animator>()->playAnimation("idle", true);
}

void GameScene::destroy()
{
    e.destroy();
}
