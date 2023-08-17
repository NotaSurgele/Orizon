#pragma once

#include "Engine/Scene/IScene.hpp"
#include "Engine/System.hpp"
#include "Engine/Core.hpp"
#include "Components/Animator.hpp"
#include "Components/EntitySignature.hpp"
#include <fstream>

class Scene : public IScene {
public:
    Scene() = default;
    ~Scene() = default;

    void create() override {};
    void update() override {};
    void destroy() override {};

    template <typename T>
    static void addEntity(T *entity)
    {
        System::addEntity(entity);
    }

    template <typename T, class... Args>
    static void addEntity(Args... args)
    {
        System::addEntity(args ...);
    }

    static Entity *getEntity(std::string const& signature)
    {
        return System::getEntity(signature);
    }

    int loadSceneFromFile(const std::string& filename)
    {
        std::string content = readConfigFile(filename);
        nlohmann::json json_content = nlohmann::json::parse(content);

        get_ressources(json_content["ressources"]);
        parse_entities(json_content["entities"]);
        return 0;
    }

    Entity *loadEntityFromFile(const std::string& filename, std::string const& name)
    {
        std::string content = readConfigFile(filename);
        nlohmann::json json_content = nlohmann::json::parse(content);

        return get_entity(json_content["entities"], name);
    }

    //template <typename T>
    //static Entity* getEntity(Signature signature)
    //{
    //    return System::getEntity<T *>(signature);
    //}

    private:
        class ComponentFactory {
            public:

                static void link_component(Entity *e, nlohmann::json const& json)
                {
                    std::string type = json["type"];

                    for (auto& it : _map) {
                        if (it.first.find(type) != std::string::npos) {
                            return it.second(e, json);
                        }
                    }
                }

            private:
                static void create_sprite(Entity *e, nlohmann::json const& json)
                {
                    sf::Texture texture = R_GET_RESSOURCE(sf::Texture, json["texture_name"]);
                    e->addComponent<Sprite>(texture);
                }

                static void create_transform(Entity *e, nlohmann::json const& json)
                {
                    sf::Vector2<float> position = sf::Vector2<float>(json["position"][0], json["position"][1]);
                    sf::Vector2<float> scale = sf::Vector2<float>(json["scale"][0], json["scale"][1]);
                    sf::Vector2<float> size = sf::Vector2<float>(json["size"][0], json["size"][1]);
                    float rotation = json["rotation"];

                    e->addComponent<Transform2D>(position.x, position.y, size.x, size.y, scale.x, scale.y, rotation);
                }

                static void create_boxcollider(Entity *e, nlohmann::json const& json)
                {
                    sf::Vector2<float> position = sf::Vector2<float>(json["position"][0], json["position"][1]);
                    sf::Vector2<float> size = sf::Vector2<float>(json["size"][0], json["size"][1]);
                    int range = json["range"];

                    e->addComponent<BoxCollider>(position, size, range);
                }

                static void create_layer(Entity *e, nlohmann::json const& json)
                {
                    std::size_t value = json["value"];
                    auto layer = e->getComponent<Layer>();

                    if (layer) {
                        layer->set(value);
                        return;
                    }
                    e->addComponent<Layer>(value);
                }

                static void create_velocity(Entity *e, nlohmann::json const& json)
                {
                    std::string type = json["value_type"];

                    if (type.find("float") != std::string::npos)
                        e->addComponent<Velocity<float>>();
                    if (type.find("int") != std::string::npos)
                        e->addComponent<Velocity<int>>();
                    if (type.find("double") != std::string::npos)
                        e->addComponent<Velocity<double>>();
                }

                static void create_animator(Entity *e, nlohmann::json const& json)
                {
                    auto animator = e->addComponent<Animator>();

                    for (auto& animation : json["animations"]) {
                        std::string name = animation["name"];
                        std::size_t frames_nb = animation["frames_number"];
                        int x = animation["rect"][0];
                        int y = animation["rect"][1];
                        int w = animation["rect"][2];
                        int h = animation["rect"][3];
                        AnimatorRect arr = AnimatorRect{x, y, w, h};
                        float speed = animation["speed"];

                        animator->newAnimation(frames_nb, arr, speed, name);
                    }
                }

                static void create_signature(Entity *e, nlohmann::json const& json)
                {
                    const std::string signature = json["signature"];

                    e->addComponent<EntitySignature>(signature);
                }

                static void create_gravity(Entity *e, nlohmann::json const& json)
                {
                    float force = json["force"];

                    e->addComponent<Gravity>(force);
                }

                static void create_view(Entity *e, nlohmann::json const& json)
                {
                    float x = json["viewport"][0];
                    float y = json["viewport"][1];
                    float w = json["viewport"][2];
                    float h = json["viewport"][3];
                    bool follow = json["follow"];

                    e->addComponent<View>(x, y, w, h, follow);
                }

            public:
                static void addComponentConstruction(std::string const& type, std::function<void(Entity *e, nlohmann::json const&)> const& constructor)
                {
                    _map[type] = constructor;
                }

            private:
                static inline std::unordered_map<std::string,
                std::function<void(Entity *e, nlohmann::json const&)>> _map = {
                    { "Transform2D", create_transform },
                    { "BoxCollider", create_boxcollider },
                    { "Sprite" , create_sprite },
                    { "Velocity", create_velocity },
                    { "Animator", create_animator },
                    { "EntitySignature", create_signature },
                    { "Gravity", create_gravity },
                    { "View", create_view },
                    { "Layer", create_layer }
                };
        };

    private:
        std::string readConfigFile(std::string const& filename)
        {
            std::string file_content = "";
            try {
                std::fstream file(filename);
                std::string buff = "";

                if (file.is_open()) {
                    while (std::getline(file, buff))
                        file_content.append(buff);
                    file.close();
                };
            } catch(std::exception &e) {
                std::cerr << e.what() << std::endl;
                throw e;
            }
            return file_content;
        }

        void get_ressources(nlohmann::json const& ressources)
        {
            for (auto& ressource : ressources) {
                std::string type = ressource["type"];
                std::string name = ressource["name"];
                std::string path = ressource["path"];

                if (type.find("Texture") != std::string::npos)
                    R_ADD_RESSOURCE(sf::Texture, name, path);
                if (type.find("Tile") != std::string::npos) {
                    float x = ressource["tile_info"][0];
                    float y = ressource["tile_info"][1];
                    float w = ressource["tile_info"][2];
                    float h = ressource["tile_info"][3];
                    R_ADD_TILE(name, path, x, y, w, h);
                }
            }
        }

        void parse_entities(nlohmann::json const& entities)
        {
            for (auto& entity : entities) {
                Entity *e = new Entity();

                for (auto& component : entity["components"])
                    ComponentFactory::link_component(e, component);
            }
        }

        Entity *get_entity(nlohmann::json const& entities, std::string const& name)
        {
            for (auto &entity : entities) {
                std::string e_name = entity["name"];

                if (e_name.find(name) == std::string::npos)
                    continue;
                Entity *e = new Entity();

                for (auto& component : entity["components"])
                    ComponentFactory::link_component(e, component);
                return e;
            }
            return nullptr;
        }

    protected:
        void addCustomComponentConstructor(std::string const& type, std::function<void(Entity *e,
                                            nlohmann::json const&)> const& constructor)
        {
            ComponentFactory::addComponentConstruction(type, constructor);
        }

};
