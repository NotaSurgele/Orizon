#include "Game/Scenes/GameScene.hpp"
#include "Engine/Components/Animator.hpp"
#include "Game/App.hpp"
#include "Engine/System.hpp"
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"

void GameScene::create()
{
   // R_ADD_RESSOURCE(sf::Texture, "hobbit", "../assets/Hobbit/pngs/hobbit_idle_spritesheet.png");
   // R_ADD_RESSOURCE(sf::Texture, "grass", "../assets/generic-rpg-tile02.png");

   // dirt.addComponent<Sprite>(&dirt, R_GET_RESSOURCE(sf::Texture, "grass"), 10, 10);
   // dirt.addComponent<Transform2D>(50, 50);
   // dirt.addComponent<BoxCollider>(sf::Vector2<float>{50, 50}, sf::Vector2<float>(16, 16));
   // player.addComponent<Sprite>(&player, R_GET_RESSOURCE(sf::Texture, "hobbit"), 10, 10);
   // player.addComponent<Animator>(&player)->newAnimation(4, AnimatorRect{0, 0, 18, 18}, .2f, "idle");
   // player.addComponent<Transform2D>();
   // player.addComponent<Velocity<float>>();
   // player.addComponent<BoxCollider>(sf::Vector2<float>{0, 0}, sf::Vector2<float>(16, 16));
   // player2.addComponent<Sprite>(&player2, R_GET_RESSOURCE(sf::Texture, "hobbit"), 10, 10);
   // player2.addComponent<Animator>(&player2)->newAnimation(4, AnimatorRect{0, 0, 18, 18}, .2f, "idle");
   // player2.addComponent<Transform2D>(500, 0);
   // player2.addComponent<Velocity<float>>();
   // player2.addComponent<BoxCollider>(sf::Vector2<float>{0, 0}, sf::Vector2<float>(16, 16));
   loadSceneFromFile("../assets/game.json");
   std::string toto = R"({ "toto": 11 })";
   nlohmann::json parsed = nlohmann::json::parse(toto);
   auto &val = parsed["toto"];
   std::cout << val << std::endl;
}

void GameScene::update()
{

   // auto velocity = player.getComponent<Velocity<float>>();
   // auto velocity2 = player2.getComponent<Velocity<float>>();
   // auto box1 = player.getComponent<BoxCollider>();
   // auto box2 = dirt.getComponent<BoxCollider>();
   // const float deltaTime = Time::deltaTime;

   // velocity2->setX(-10);
   // if (Input::isActionPressed("MoveUp"))
   //     velocity->setY(-100);
   // else if (Input::isActionPressed("MoveDown"))
   //     velocity->setY(100);
   // else
   //     velocity->setY(0);

   // if (Input::isActionPressed("MoveLeft"))
   //     velocity->setX(-100);
   // else if (Input::isActionPressed("MoveRight"))
   //     velocity->setX(100);
   // else
   //     velocity->setX(0);
   // if (Input::isActionPressed("Exit"))
   //     CLOSE();
   // if (Input::isActionPressed("Refresh"))
   //     CORE->loadInputFromFile(INPUT_FILE);
   // player.getComponent<Animator>()->playAnimation("idle", true);
   // DRAW(box1);
   // DRAW(box2);
}

void GameScene::destroy()
{
}
