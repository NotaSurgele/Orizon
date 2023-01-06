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

        for (float y = 0; y <= 256; y += 1) {
            std::vector<float> map;

            for (float x = 0; x <= 256; x += 1) {
                float pos = stb_perlin_noise3_seed(x / 100.0f, y / 100.0f, 0
                            ,0, 0, 0, std::rand() % 4000);
                std::cout << pos << std::endl;
                map.push_back(pos);
            }
            _heightMap.push_back(map);
        }

        int i = 0;
        int j = 0;

        for (float y = 0; y <= 800; y += 16) {
            for (float x = 0; x <= 600; x += 16) {
                float val = _heightMap[i][j];

                if (val > .5f) {
                    Entity *e = loadEntityFromFile("../assets/entities.json", "grass");
                    auto transform = e->getComponent<Transform2D>();

                    transform->position.x = x + offset;
                    transform->position.y = y + offset;
                    _blocks.push_back(e);
                } else if (val < .1f) {
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
