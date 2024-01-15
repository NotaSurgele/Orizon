#include "Scene.hpp"
#include "Core.hpp"

void Scene::ComponentFactory::create_sprite(Entity *e, nlohmann::json const& json)
{
    std::string textureName = json["texture_name"];
    sf::Texture texture = R_GET_RESSOURCE(sf::Texture, textureName);

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
    sf::Texture lightTexture = R_GET_RESSOURCE(sf::Texture, json["texture_name"]);
    auto *sprite = new Sprite(lightTexture);

    if (json.contains("intensity"))
        intensity = json["intensity"];
    e->addComponent<Light>(sprite, intensity);
}

void Scene::ComponentFactory::create_sound(Entity *e, nlohmann::json const& json)
{
    sf::SoundBuffer buffer = R_GET_RESSOURCE(sf::SoundBuffer, json["sound_name"]);
    bool loop = json["loop"];

    e->addComponent<Sound>(json["sound_name"])->setBuffer(buffer)
                            ->setLoop(loop);
}

void Scene::get_ressources(nlohmann::json const& ressources)
{
    for (auto& ressource : ressources) {
        std::string type = ressource["type"];
        std::string path = ressource["path"];
        std::string name = "";

        if (ressource.contains("name"))
            name = ressource["name"];
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
        else if (type.find("Script") != std::string::npos)
            R_ADD_SCRIPT(path);
    }
}