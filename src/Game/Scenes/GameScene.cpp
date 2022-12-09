#include "Game/Scenes/GameScene.hpp"
#include "Engine/Components/Animator.hpp"
#include "Game/App.hpp"
#include "Engine/System.hpp"
#include "Components/Velocity.hpp"

void GameScene::create()
{
    R_ADD_RESSOURCE(sf::Texture, "hobbit", "../assets/Hobbit/pngs/hobbit_idle_spritesheet.png");
    R_ADD_RESSOURCE(sf::Texture, "grass", "../assets/generic-rpg-tile02.png");

    e.addComponent<Sprite>(&e, R_GET_RESSOURCE(sf::Texture, "hobbit"), 10, 10);
    e.addComponent<Animator>(&e)->newAnimation(4, AnimatorRect{0, 0, 64, 64}, .2f, "idle");
    e.addComponent<Transform2D>();
    float x = 0;
    float y = 0;
    int numb = 0;
    for (std::size_t i = 0; i < 500; i++) {
        Entity *tile = new Entity();

        tile->addComponent<Sprite>(tile, R_GET_RESSOURCE(sf::Texture, "grass"));
        tile->addComponent<Transform2D>(x, y);
        if ((numb % 10) == 0) {
            x = 0.0f;
            y += 16.f;
            numb = 0;
        }
        numb++;
        x += 16;
        System::addEntity<Entity>(tile);
    }
    System::addEntity<Entity>(&e);
}

void GameScene::update()
{
    auto transform = e.getComponent<Transform2D>();
    const float deltaTime = Time::deltaTime;

    if (Input::isActionPressed("MoveDown"))
        transform->position.y += 100.0f * deltaTime;
    if (Input::isActionPressed("MoveUp"))
        transform->position.y += -100.0f * deltaTime;
    if (Input::isActionPressed("MoveLeft"))
        transform->position.x += -100.0f * deltaTime;
    if (Input::isActionPressed("MoveRight"))
        transform->position.x += 100.0f * deltaTime;
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
