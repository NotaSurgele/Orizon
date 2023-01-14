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

#include "OpenSimplexNoise.hpp"

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

        float offset = 700;

        _heightMap.clear();

        for (auto block : _blocks) {
            if (block)
                block->destroy();
        }
        _blocks.clear();

        //PerlinNoise generation https://www.youtube.com/watch?v=l5KVBDOsHfg, https://www.youtube.com/watch?v=MTNt32_NQlc, https://www.youtube.com/watch?v=lhWjEd8I4fM
        //OpenSimplex Noise generation https://github.com/deerel/OpenSimplexNoise
        //QuadTree https://www.youtube.com/watch?v=OJxEcs0w_kE
        for (int x = 0; x < 50; x++) {
            std::vector<int> map;
            for (int y = 0; y < 50; y++) {
                map.push_back(0);
            }
            _heightMap.push_back(map);
        }

        int height = 0;
        float smooth = 70;
        float h = 50;

        float modifier = .1f;

        int64_t seed = std::rand() % 4000;
        for (int x = 0; x < 50; x++) {
            OpenSimplexNoise::Noise _noise(seed);

            // height = round(stb_perlin_noise3_seed(x / smooth, 0, 0, 0,
            //             0, 0, seed) * h / 2);

            height = round(_noise.eval(x / smooth, 0) * h / 2);

            height += h / 2;
            for (int y = 0; y < height; y++) {
                // int cave = round(stb_perlin_noise3_seed((x * modifier) + seed, (y * modifier) + seed, 0, 0, 0, 0, seed));
                int cave = round(_noise.eval((x * modifier), (y * modifier)));

                _heightMap[x][y] = (cave >= 1) ? 0 : 1;
            }
        }

        float i = 0;
        float j = 0;

        for (int x = 0; x < 50; x++) {
            for (int y = 0; y < 50; y++) {
                if (_heightMap[x][y] == 1) {
                    Entity *e = loadEntityFromFile("../assets/entities.json", "grass");
                    auto transform = e->getComponent<Transform2D>();

                    transform->position.x = i;
                    transform->position.y = -j + offset;
                    _blocks.push_back(e);
                }
                j += 16;
            }
            j = 0;
            i += 16;
        }
    }
    player->getComponent<Animator>()->playAnimation("idle", true);
    DRAW(box1);
}

void GameScene::destroy()
{
}
