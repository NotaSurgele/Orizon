#pragma once
#include "IComponent.hpp"
#include "Entity.hpp"
#include "sol/sol.hpp"
#include "Transform2D.hpp"
#include "Animator.hpp"
#include "BoxCollider.hpp"
#include "Gravity.hpp"
#include "Layer.hpp"
#include "Light.hpp"
#include "OrizonMusic.hpp"
#include "Sound.hpp"
#include "Sprite.hpp"
#include "Tag.hpp"
#include "Velocity.hpp"
#include "View.hpp"
#include "Input.hpp"
#include "Time.hpp"
#include "System.hpp"
#include "Canvas.hpp"
#include <unordered_map>
#include <variant>

#include <string>

typedef unsigned int uint;

class Script : public IComponent {
public:
    Script(Entity *e, const std::string& scriptPath);
    Script() = delete;
    ~Script() = default;

    void update();
    void start();
    void destroyObjectInstance();

    void destroy() override {
        this->destroyObjectInstance();
    }

    const std::string& getFile() const
    {
        return _filepath;
    }

    void reload();

    void setScript(const std::string& filePath);

private:
    void create(const std::string& path, bool insert=true);

    // System
    void registerInputSystem();

    // base types
    void registerVectorType();
    void registerColorType();
    void registerRectType();
    void registerSystemType();
    void registerTileMap();
    void registerUtilsType();
    void registerResourceManager();
    void registerDrawableType();
    void registerCoreType();
    void registerCanvasTypes();
    void registerBaseTypes();

    // register component
    void registerTransform2DComponent();
    void registerAnimatorComponent();
    void registerBoxColliderComponent();
    void registerGravityComponent();
    void registerLayerComponent();
    void registerLightComponent();
    void registerOrizonMusicComponent();
    void registerSoundComponent();
    void registerSpriteComponent();
    void registerVelocityComponent();
    void registerTagComponent();
    void registerViewComponent();
    void registerScriptComponent();
    void registerCanvasComponent();
    void registerComponentsType();

    void registerEntityFunction();

    void handleTypeTransformation(std::vector<sol::object>& modifiedArgs, int i);

    template <typename T>
    sol::object convertUserDataToTypes(const sol::userdata& ud)
    {
        if (ud.is<T>()) {
            return sol::make_object(*_state, ud.as<T>());
        }
        return sol::nil;
    }

    std::variant<Script *,Entity *, sf::FloatRect,sf::Vector2f,
    sf::Vector2i,sf::Vector2u, sf::IntRect,sf::Color,
    sol::nil_t, sol::metatable,sol::object>getTableValue(const sol::object& res);

    sol::metatable deserializeTable(const sol::metatable& table);

    std::variant<
    Script *,
    Entity *, sf::FloatRect,
    sf::Vector2f, sf::Vector2i,
    sf::Vector2u, sf::IntRect,
    sf::Color, sol::nil_t, sol::metatable,
    sol::object> call(const std::string& function, sol::variadic_args args);

private:
    std::vector<std::function<sol::object(sol::userdata&)>> typesArray = {
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<Entity *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<Transform2D *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<Animator *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<BoxCollider *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<Gravity *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<Layer *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<Light *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<OrizonMusic *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<Sound *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<Sprite *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<Velocity *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<Tag *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<View *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<Script *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<Canvas *>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<sf::Vector2f>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<sf::Vector2i>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<sf::Vector2u>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<sf::Color>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<sf::FloatRect>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<sf::IntRect>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<std::string>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<int>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<float>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<sol::table>(ud); }}
    };

protected:
    bool _start = false;
    Entity *_self = nullptr;
    sol::state *_state = nullptr;
    std::string _filepath;
};