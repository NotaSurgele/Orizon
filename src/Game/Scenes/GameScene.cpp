#include "Game/Scenes/GameScene.hpp"
#include "Engine/Components/Animator.hpp"
#include "Game/App.hpp"
#include "Engine/System.hpp"
#include "Game/CustomComponents/CharacterController.hpp"
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Gravity.hpp"

#define STB_PERLIN_IMPLEMENTATION
#include "external/stb_perlin.hpp"

#include <random>
#include <math.h>

void GameScene::create()
{
    addCustomComponentConstructor("CharacterController", [](Entity *e, nlohmann::json const& json) {
        auto speed = json["speed"];

        e->addCustomComponent<CharacterController>(speed);
    });
    loadSceneFromFile("../assets/game.json");
    player = getEntity("player");
}

void GameScene::update()
{
    auto box1 = player->getComponent<BoxCollider>();

    if (Input::isActionPressed("Exit"))
        CLOSE();
    if (Input::isActionPressed("Refresh"))
        CORE->loadInputFromFile(INPUT_FILE);
    if (Input::isKeyDown("Space")) {

        float offset = 200;

        _heightMap.clear();

        for (auto block : _blocks) {
            if (block)
                block->destroy();
        }
        _blocks.clear();

        for (float y = 0; y <= 800; y += 16) {
            std::vector<int> map;

            for (float x = 0; x <= 600; x += 16) {
                int pos = round(stb_perlin_noise3_seed(x / 100, y / 100 , 0, 0, 0, 0, std::rand() % 4000) * 2);

                map.push_back(pos);
            }
            _heightMap.push_back(map);
        }

        int i = 0;
        int j = 0;

        for (float y = 0; y <= 800; y += 16) {
            for (float x = 0; x <= 600; x += 16) {
                if (_heightMap[i][j] == 0) {
                    Entity *e = loadEntityFromFile("../assets/entities.json", "block");
                    auto transform = e->getComponent<Transform2D>();

                    transform->position.x = x + offset;
                    transform->position.y = y + offset;
                    _blocks.push_back(e);
                }
                j++;
            }
            j = 0;
            i++;
        }
    }
    player->getComponent<Animator>()->playAnimation("idle", true);
    DRAW(box1);
}

void GameScene::destroy()
{
}
