#include "Game/Scenes/GameScene.hpp"
#include "Engine/Components/Animator.hpp"
#include "Game/App.hpp"
#include "Engine/System.hpp"
#include "Game/CustomComponents/CharacterController.hpp"
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Gravity.hpp"
#include "Components/Light.hpp"

#define STB_PERLIN_IMPLEMENTATION
#include "external/stb_perlin.hpp"

#include "OpenSimplexNoise.hpp"

#include <random>
#include <math.h>

void GameScene::create()
{
    // _particles = Particles(10);
    // _particles.setShape(ParticleShape::SQUARE, 10);
    addCustomComponentConstructor("CharacterController", [](Entity *e, nlohmann::json const& json) {
        auto speed = json["speed"];

        e->addCustomComponent<CharacterController>(speed);
    });

    loadSceneFromFile("../assets/game.json");
    player = getEntity("player");
    player->addComponent<Light>();
    player->addComponent<Sound>();
    player->getComponent<OrizonMusic>();
}

void GameScene::update()
{
    // _particles.play(false);
    auto box1 = player->getComponent<BoxCollider>();

    if (Input::isActionPressed("Exit"))
        CLOSE();
    if (Input::isActionPressed("Refresh"))
        CORE->loadInputFromFile(INPUT_FILE);
    if (Input::isKeyDown("Space")) {
        layer = new TileMap(0, 0, 3000, 3000, 16, 16);
        float offset = 700;
        int chunks = 1;
        float w = 100;
        _heightMap.clear();

        for (auto block : _blocks) {
            if (block)
                block->destroy();
        }
        _blocks.clear();

        //PerlinNoise generation https://www.youtube.com/watch?v=l5KVBDOsHfg, https://www.youtube.com/watch?v=MTNt32_NQlc, https://www.youtube.com/watch?v=lhWjEd8I4fM
        //OpenSimplex Noise generation https://github.com/deerel/OpenSimplexNoise
        //QuadTree https://www.youtube.com/watch?v=OJxEcs0w_kE
        //lua implentation https://github.com/ThePhD/sol2
        for (int i = 0; i < chunks ; i++) {
            for (int x = 0; x < w; x++) {
                std::vector<int> map;
                for (int y = 0; y < w; y++) {
                    map.push_back(0);
                }
                _heightMap.push_back(map);
            }
        }

        int height = 0;
        float smooth = 70;
        float h = w;

        float modifier = .09f;

        int64_t seed = std::rand() % 4000;
        for (int i = 0; i < chunks; i++) {
            for (int x = 0; x < w; x++) {
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
        }

        float i = 0;
        float j = 0;

        for (int t = 0; t < chunks; t++) {
            for (int x = 0; x < w; x++) {
                for (int y = 0; y < w; y++) {
                    if (_heightMap[x][y] == 1) {
                        Entity *e = loadEntityFromFile("../assets/entities.json", "grass");
                        auto transform = e->getComponent<Transform2D>();

                        transform->position.x = i;
                        transform->position.y = -j + offset;
                        _blocks.push_back(e);
                        layer->emplaceEntity(e);
                    }
                    j += 16;
                }
                j = 0;
                i += 16;
            }
        }
        // System::refresh_quad();
    }
    player->getComponent<Animator>()->playAnimation("idle", true);
    // for (auto ray : _rayCaster) {
    //     ray.setPosition(sf::Mouse::getPosition(Window.getSFMLRenderWindow()));
    //     if (ray.hit(wall2))
    //         std::cout << "oui" << std::endl;
    //     else
    //         ray.hit(wall);
    //     ray.show(2);
    // }
    // ray.hit()
    // player->getComponent<Sprite>()->setTexture(test.getTexture());
    // DRAW(box1);
    // DRAW(wall);
    // DRAW(wall2);
}

void GameScene::destroy()
{
}
