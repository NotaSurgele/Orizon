#include "Game/Scenes/GameScene.hpp"
#include "Engine/Components/Animator.hpp"
#include "Game/App.hpp"
#include "Engine/System.hpp"
#include "Game/CustomComponents/CharacterController.hpp"
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Gravity.hpp"

void GameScene::create()
{
    R_ADD_RESSOURCE(sf::Texture, "hobbit", "../assets/Hobbit/pngs/hobbit_idle_spritesheet.png");

    player.addComponent<Sprite>(R_GET_RESSOURCE(sf::Texture, "hobbit"), 10, 10);
    player.addComponent<Animator>()->newAnimation(4, AnimatorRect{0, 0, 18, 18}, .2f, "idle");
    player.addComponent<Velocity<float>>();
    player.addComponent<BoxCollider>(sf::Vector2<float>{0, 0}, sf::Vector2<float>(16, 16));
    player.addComponent<Transform2D>();
    player.addComponent<Gravity>();
    player.addCustomComponent<CharacterController>();
    // loadSceneFromFile("../assets/game.json");
    // std::string toto = R"({ "toto": 11 })";
    // nlohmann::json parsed = nlohmann::json::parse(toto);
    // auto &val = parsed["toto"];
    // std::cout << val << std::endl;
}

void GameScene::update()
{
    auto box1 = player.getComponent<BoxCollider>();

    if (Input::isActionPressed("Exit"))
        CLOSE();
    if (Input::isActionPressed("Refresh"))
        CORE->loadInputFromFile(INPUT_FILE);
    player.getComponent<Animator>()->playAnimation("idle", true);
    DRAW(box1);
}

void GameScene::destroy()
{
}
