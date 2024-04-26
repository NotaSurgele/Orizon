#include "Scene.hpp"
#include "Core.hpp"

void Scene::ComponentFactory::create_sprite(Entity *e, nlohmann::json const& json)
{
    std::string textureName = json["texture_name"];
    sf::Texture *texture = R_GET_RESSOURCE(sf::Texture, textureName);

    e->addComponent<Sprite>(texture)->setTextureName(textureName);
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

void Scene::get_ressources(nlohmann::json const& ressources)
{
    for (auto& ressource : ressources) {
        std::string type = ressource["type"];
        std::string path = "";
        std::string name = "";

        if (ressource.contains("name"))
            name = ressource["name"];
        if (ressource.contains("path"))
            path = ressource["path"];
        if (type.find("Texture") != std::string::npos)
            R_ADD_RESSOURCE(sf::Texture, name, path);
        else if (type.find("Tile") != std::string::npos) {
            float x = ressource["tile_info"][0];
            float y = ressource["tile_info"][1];
            float w = ressource["tile_info"][2];
            float h = ressource["tile_info"][3];
            R_ADD_TILE(name, path, x, y, w, h);
        } else if (type.find("Sound") != std::string::npos)
            R_ADD_RESSOURCE(sf::SoundBuffer, name, path);
        else if (type.find("Music") != std::string::npos)
            R_ADD_MUSIC(name, path);
        else if (type.find("Entities") != std::string::npos)
            _entitiesPath = path;
        else if (type.find("Shader") != std::string::npos) {
            auto& vertex = ressource["vertex"];
            auto& fragment = ressource["fragment"];

            R_ADD_SHADER(name, vertex, fragment);
        }
    }
}