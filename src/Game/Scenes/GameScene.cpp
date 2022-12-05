#include "Game/Scenes/GameScene.hpp"
#include "Engine/Components/Animator.hpp"
#include "Game/App.hpp"
#include "Engine/System.hpp"
#include "Components/Velocity.hpp"

void GameScene::create()
{
    R_ADD_RESSOURCE(sf::Texture, "hobbit", "../assets/Hobbit/pngs/hobbit_idle_spritesheet.png");
    //e.addComponent<Sprite>(&e, R_GET_RESSOURCE(sf::Texture, "hobbit"), 10, 10);
    e.addComponent<Animator>(&e)->newAnimation(4, AnimatorRect{0, 0, 64, 64}, .2f, "idle");
    //e.addComponent<Transform2D>();
    System::addEntity<Entity>(&e);
}

void GameScene::update()
{
   // auto transform = e.getComponent<Transform2D>();
   // const float deltaTime = Time::deltaTime;

   // if (Input::isActionPressed("MoveDown"))
   //     transform->position.y += 100.0f * deltaTime;
   // if (Input::isActionPressed("MoveUp"))
   //     transform->position.y += -100.0f * deltaTime;
   // if (Input::isActionPressed("MoveLeft"))
   //     transform->position.x += -100.0f * deltaTime;
   // if (Input::isActionPressed("MoveRight"))
   //     transform->position.x += 100.0f * deltaTime;
    if (Input::isActionPressed("Exit"))
        CLOSE();
    if (Input::isActionPressed("Refresh"))
        CORE->loadInputFromFile(INPUT_FILE);
    //if (Input::isActionPressed("Animate"))
    //    e.getComponent<Animator>()->playAnimation("idle", true);
}

void GameScene::destroy()
{
    e.destroy();
}
