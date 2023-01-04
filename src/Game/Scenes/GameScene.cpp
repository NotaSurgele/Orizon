#include "Game/Scenes/GameScene.hpp"
#include "Engine/Components/Animator.hpp"
#include "Game/App.hpp"
#include "Engine/System.hpp"
#include "Game/CustomComponents/CharacterController.hpp"
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Gravity.hpp"
#include "external/db_perlin.hpp"

#include <random>

void GameScene::create()
{
    addCustomComponentConstructor("CharacterController", [](Entity *e, nlohmann::json const& json) {
        e->addCustomComponent<CharacterController>();
    });
    loadSceneFromFile("../assets/game.json");
    player = getEntity("player");

    const siv::PerlinNoise::seed_type seed = 1234u;
    const siv::PerlinNoise noise{ seed };

    for (float y = 0; y <= 500; y += 16) {
        std::vector<int> map;

        for (float x = 0; x <= 600; x += 16) {
            int pos = noise.octave2D_01((x * 50.0f), (y * 50.0), 4);
            map.push_back(pos);
        }
        _heightMap.push_back(map);
    }
}

void GameScene::update()
{
    auto box1 = player->getComponent<BoxCollider>();

    if (Input::isActionPressed("Exit"))
        CLOSE();
    if (Input::isActionPressed("Refresh"))
        CORE->loadInputFromFile(INPUT_FILE);
    if (Input::isKeyDown("Space")) {

        _heightMap.clear();

        for (auto block : _blocks) {
            if (block)
                block->destroy();
        }
        _blocks.clear();

        const siv::PerlinNoise::seed_type seed = 1234u;
        const siv::PerlinNoise noise{ seed };

        for (float y = 0; y <= 500; y += 16) {
            std::vector<int> map;

            for (float x = 0; x <= 600; x += 16) {
                int pos = noise.octave2D_01((x * 50.0f), (y * 50.0), 4) * 2;
                std::cout << pos << std::endl;
                map.push_back(pos);
            }
            _heightMap.push_back(map);
        }

        int i = 0;
        int j = 0;

        for (float y = 0; y <= 500; y += 16) {
            for (float x = 0; x <= 600; x += 16) {

                if (_heightMap[i][j] == 1) {
                    Entity *e = loadEntityFromFile("../assets/entities.json", "block");
                    auto transform = e->getComponent<Transform2D>();

                    transform->position.x = x;
                    transform->position.y = y;
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
