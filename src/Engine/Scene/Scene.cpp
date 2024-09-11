#include "Scene.hpp"
#include "Core.hpp"

void Scene::ComponentFactory::create_sprite(Entity *e, nlohmann::json const& json)
{
    std::string textureName = json["texture_name"];
    auto texture = R_GET_RESSOURCE(sf::Texture, textureName);

    auto sprite = e->addComponent<Sprite>(texture);
    sprite->setTextureName(textureName);
    if (json.contains("shader")) {
        auto shader = R_GET_RESSOURCE(Shader, json["shader"]["name"]);
        sprite->attachShader(shader);
    }
}

void Scene::ComponentFactory::create_music(Entity *e, nlohmann::json const& json)
{
    sf::Music *buffer = R_GET_MUSIC(json["music_name"]);
    bool loop = json["loop"];

    e->addComponent<OrizonMusic>(json["music_name"])->setMusic(buffer)
                                    ->setLoop(loop);
}

void Scene::ComponentFactory::create_light(Entity *e, nlohmann::json const& json)
{
    float intensity = .4f;
    sf::Texture *lightTexture = R_GET_RESSOURCE(sf::Texture, json["texture_name"]);
    auto *sprite = new Sprite(lightTexture);

    if (json.contains("intensity"))
        intensity = json["intensity"];
    e->addComponent<Light>(sprite, intensity);
}

void Scene::ComponentFactory::create_sound(Entity *e, nlohmann::json const& json)
{
    sf::SoundBuffer *buffer = R_GET_RESSOURCE(sf::SoundBuffer, json["sound_name"]);
    bool loop = json["loop"];

    e->addComponent<Sound>(json["sound_name"])->setBuffer(buffer)
                            ->setLoop(loop);
}

void Scene::ComponentFactory::create_canvas(Entity *e, const nlohmann::json &json)
{
    auto canvas = e->addComponent<Canvas>();
    auto canvasObjects = json["canvas_objects"];

    for (auto& obj : canvasObjects) {
        std::string type = obj["type"];

        auto position = sf::Vector2f(obj["position"][0], obj["position"][1]);
        auto coordType = obj["coord_type"];

        // [TODO] refactor in to unordered_map
        if (type.find("Button") != std::string::npos) {
            try {
                auto texture = R_GET_RESSOURCE(sf::Texture, obj["texture_name"]);
                auto size = sf::Vector2f(obj["size"][0], obj["size"][1]);

                canvas->addButton(position, size, texture)->coordType = coordType;
            } catch (std::exception& err) {
                std::cerr << "[SCENE] CANVAS Creating Button error " << err.what() << std::endl;
            }
        } else if (type.find("Text") != std::string::npos) {
            try {
                std::string content = obj["content"];
                std::size_t size = obj["font_size"];

                canvas->addText(content, position, size)->coordType = coordType;
            } catch (std::exception& err) {
                std::cerr << "[SCENE] CANVAS Creating Text error " << err.what() << std::endl;
            }
        } else if (type.find("Image") != std::string::npos) {
            try {
                auto texture = R_GET_RESSOURCE(sf::Texture, obj["texture_name"]);
                auto size = sf::Vector2f(obj["size"][0], obj["size"][1]);

                canvas->addImage(texture, position, size)->coordType = coordType;
            } catch (std::exception& err) {
                std::cerr << "[SCENE] CANVAS Creating Text error " << err.what() << std::endl;
            }
        } else {
            std::cerr << "[SCENE CREATE COMPONENT] canvasObject type "
                    << type << " does not exist !" << std::endl;
            continue;
        }
    }
}

void Scene::ComponentFactory::create_particles_emitter(Entity *e, const nlohmann::json &json) {
    auto emitter = e->addComponent<ParticlesEmitter>();

    try {
        for (auto &particle: json["particles"]) {
            std::string path = particle["path"];
            std::string name = particle["name"];

            Particle p = Particle(path);
            emitter->particles.insert(std::pair<std::string, Particle>(name, p));
            std::cout << "Create a particle emitter with particle" << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Scene::get_ressources(nlohmann::json const& ressources)
{
    static std::unordered_map<std::string, std::function<void(std::string& name, const nlohmann::json& resource)>> resourceMap = {
        {
        "Texture", [&](std::string &name, const nlohmann::json &resource) {
                try {
                    std::string path = resource["path"];

                    R_ADD_RESSOURCE(sf::Texture, name, path);
                } catch (std::exception &e) {
                    std::cerr << "[SCENE] missing texture parameter in json " << _sceneFile << " " << e.what()
                              << std::endl;
                }
            }
        },
        {
        "Tile", [&](std::string &name, const nlohmann::json &resource) {
                try {
                    std::string path = resource["path"];
                    float x = resource["tile_info"][0];
                    float y = resource["tile_info"][1];
                    float w = resource["tile_info"][2];
                    float h = resource["tile_info"][3];

                    R_ADD_TILE(name, path, x, y, w, h);
                } catch (std::exception& e) {
                    std::cerr << "[SCENE] missing texture parameter in json " << _sceneFile << " " << e.what() << std::endl;
                }
            }
        },
        {
        "Sound", [&](std::string &name, const nlohmann::json &resource) {
                try {
                    std::string path = resource["path"];

                    R_ADD_RESSOURCE(sf::SoundBuffer, name, path);
                } catch (std::exception& e) {
                    std::cerr << "[SCENE] missing sound parameter in json file " << _sceneFile << " " << e.what() << std::endl;
                }
            }
        },
        {
        "Music", [&] (std::string& name, const nlohmann::json& resource) {
                try {
                    std::string path = resource["path"];

                    R_ADD_MUSIC(name, path);
                } catch (std::exception& e) {
                    std::cerr << "[SCENE] missing music parameter in json file " << _sceneFile << " " << e.what() << std::endl;
                }
            }
        },
        {
        "Entities", [&] (std::string& name, const nlohmann::json& resource) {
                try {
                    std::string path = resource["path"];

                    _entitiesPath = path;
                } catch (std::exception& e) {
                    std::cerr << "[SCENE] missing entities parameter in json file " << _sceneFile << " " << e.what() << std::endl;
                }
            }
        },
        {
          "Shader", [&] (std::string& name, const nlohmann::json& resource) {
                try {
                    std::string vertex;
                    std::string fragment;

                    if (resource.contains("vertex"))
                        vertex = resource["vertex"];
                    if (resource.contains("fragment"))
                        fragment = resource["fragment"];
                    R_ADD_SHADER(name, vertex, fragment);
                } catch (std::exception& e) {
                    std::cerr << "[SCENE] missing shader parameter in json file " << _sceneFile << " " << e.what() << std::endl;
                }
            }
        }
    };
    for (auto& ressource : ressources) {
        std::string type = ressource["type"];
        std::string name;

        if (ressource.contains("name")) name = ressource["name"];
        for (auto& it : resourceMap) {
            if (it.first == type) {
                it.second(name, ressource);
                break;
            }
        }
    }
}