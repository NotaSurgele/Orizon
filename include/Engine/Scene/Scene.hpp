#pragma once
#include "EngineHud.hpp"
#include "Engine/System.hpp"
#include "Engine/Scene/IScene.hpp"
#include "Components/Animator.hpp"
#include "Components/Tag.hpp"
#include "Components/Light.hpp"
#include "Components/Script.hpp"
#include "json.hpp"
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

        try {
            _sceneContent = nlohmann::json::parse(content);

        } catch (std::exception& msg) {
            std::cerr << "[LOAD_SCENE_FROM_FILE] " << msg.what() << std::endl;
            return -1;
        }

        get_ressources(_sceneContent["resources"]);
        parse_entities(_sceneContent["entities"]);
        _sceneFile = filename;
        return 0;
    }

    const std::string& getSceneFilepath() const { return _sceneFile; }
    nlohmann::json& getSceneContent() { return _sceneContent; }
    static inline Entity *loadEntityFromFilepath(const std::string& filename, std::string const& name)
    {
        std::string content = readConfigFile(filename);

        try {
            nlohmann::json json_content = nlohmann::json::parse(content);

            return get_entity(json_content["entities"], name);
        } catch (std::exception& msg) {
            std::cerr << "[LOAD_ENTITY_FROM_FILE] " << msg.what() <<
                " entity file doesn't exist " << filename << std::endl;
            return nullptr;
        }

    }

    static inline Entity *loadEntityFromFile(const std::string& content, std::string const& name)
    {
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
                static void create_sprite(Entity *e, nlohmann::json const& json);

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
                    std::string type_string = json["collision_type"];

                    const std::unordered_map<std::string, BoxCollider::Type> types = {
                        { "Dynamic", BoxCollider::Type::DYNAMIC },
                        { "Static", BoxCollider::Type::STATIC },
                    };
                    BoxCollider::Type type = types.at(type_string);
                    auto collider = e->addComponent<BoxCollider>(position, size, range);
                    collider->setOffset(position);
                    collider->setType(type);
                    if (json.contains("isTrigger")) {
                        bool trigger = json["isTrigger"];
                        collider->setTrigger(trigger);
                    }
                }

                static void create_layer(Entity *e, nlohmann::json const& json)
                {
                    std::size_t value = json["value"];
                    auto layer = e->getComponent<Layer>();

                    layer->set(value);
                }

                static void create_velocity(Entity *e, nlohmann::json const& json)
                {
                    e->addComponent<Velocity>();
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

                static void create_tag(Entity *e, nlohmann::json const& json)
                {
                    const std::string signature = json["tag"];

                    e->addComponent<Tag>(signature);
                }

                static void create_gravity(Entity *e, nlohmann::json const& json)
                {
                    float force = json["force"];

                    e->addComponent<Gravity>(force);
                }

                static void create_view(Entity *e, nlohmann::json const& json)
                {
                    float x = json["view_bounds"][0];
                    float y = json["view_bounds"][1];
                    float w = json["view_bounds"][2];
                    float h = json["view_bounds"][3];
                    bool follow = json["follow"];

                    auto view = e->addComponent<View>(x, y, w, h, follow);

                    if (json.contains("viewport")) {
                        float viewportPosX = json["viewport"][0];
                        float viewportPosY = json["viewport"][1];
                        float viewportWidth = json["viewport"][2];
                        float viewportHeight = json["viewport"][3];

                        view->setViewPort({ viewportPosX, viewportPosY,
                                                viewportWidth, viewportHeight });
                    }
                }

                static void create_sound(Entity *e, nlohmann::json const& json);

                static void create_music(Entity *e, nlohmann::json const& json);

                static void create_light(Entity *e, nlohmann::json const& json);

                static void create_script(Entity *e, const nlohmann::json& json)
                {
                    std::string path = json["path"];

                    e->addComponent<Script>(path);
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
                    { "Transform2D", create_transform },
                    { "BoxCollider", create_boxcollider },
                    { "Sprite" , create_sprite },
                    { "Velocity", create_velocity },
                    { "Animator", create_animator },
                    { "Tag", create_tag },
                    { "Gravity", create_gravity },
                    { "View", create_view },
                    { "Layer", create_layer },
                    { "Sound", create_sound },
                    { "Music", create_music },
                    { "Light", create_light },
                    { "Script", create_script }
                };
        };

    private:
        static inline std::string readConfigFile(std::string const& filename)
        {
            std::string file_content;
            try {
                std::fstream file(filename);
                std::string buff;

                if (file.is_open()) {
                    while (std::getline(file, buff))
                        file_content.append(buff);
                    file.close();
                }
            } catch(std::exception &e) {
                std::cerr << e.what() << std::endl;
                throw e;
            }
            return file_content;
        }

        void get_ressources(nlohmann::json const& ressources);

        void parse_entities(nlohmann::json const& entities)
        {
            for (auto& entity : entities) {
                if (entity.is_string()) {
                    loadEntityFromFilepath(_entitiesPath, entity);
                    continue;
                }
                auto *e = new Entity();
                std::vector<nlohmann::json> scripts;


                for (auto& component : entity["components"]) {
                    auto type = component["type"];

                    if (type.get<std::string>().find("Script") != std::string::npos) {
                        scripts.push_back(component);
                        continue;
                    }
                    ComponentFactory::link_component(e, component);
                }
                for (auto& script : scripts) {
                    ComponentFactory::link_component(e, script);
                }
                System::pushEntity(e);
                System::forceUpdate(e);

                if (ENGINE_MODE) EngineHud::registerSavedEntity(e);
            }
        }

        static inline Entity *get_entity(nlohmann::json const& entities, std::string const& name)
        {
            for (auto &entity : entities) {
                std::string e_name = entity["name"];

                if (e_name.find(name) == std::string::npos)
                    continue;
                auto *e = new Entity();
                std::vector<nlohmann::json> scripts;

                for (auto& component : entity["components"]) {
                    auto type = component["type"];

                    if (type.get<std::string>().find("Script") != std::string::npos) {
                        scripts.push_back(component);
                        continue;
                    }
                    ComponentFactory::link_component(e, component);
                }
                for (auto& script : scripts) {
                    ComponentFactory::link_component(e, script);
                }
                System::pushEntity(e);
                System::forceUpdate(e);
                if (ENGINE_MODE) EngineHud::registerSavedEntity(e);
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

private:
    std::string _sceneFile;
    std::string _entitiesPath;
    nlohmann::json _sceneContent;
};
