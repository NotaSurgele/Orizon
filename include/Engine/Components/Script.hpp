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
#include <unordered_map>
#include <string>

typedef unsigned int uint;

class Script : public IComponent {
public:
    Script(Entity *e, const std::string& scriptPath);
    Script() = delete;
    ~Script() = default;

    void update();
    void start();

    void destroy() override {}

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

    template <typename T>
    T call(const std::string& function, sol::variadic_args args)
    {
        try {
            sol::function f = (*_state)[function];
            if (!f.valid()) {
                std::cerr << "Not a valid function name " << function << std::endl;
            }
            // Convert userdata arguments to Entity* if applicable
            std::vector<sol::object> modifiedArgs(args.begin(), args.end());
            for (size_t i = 0; i < modifiedArgs.size(); ++i) {
                handleTypeTransformation(modifiedArgs, i);
            }
            sol::object res = f(sol::as_args(modifiedArgs));

            if (res.is<sol::nil_t>()) {
                return T();
            }
            return res.as<T>();
        } catch (sol::error& error) {
            std::cerr << error.what() << std::endl;
        }
        return T();
    }

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
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<sf::Vector2f>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<sf::Vector2i>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<sf::Vector2u>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<sf::Color>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<sf::FloatRect>(ud); }},
            { [&](const sol::userdata& ud) { return convertUserDataToTypes<sf::IntRect>(ud); }},
    };

protected:
    bool _start = false;
    Entity *_self = nullptr;
    sol::state *_state = nullptr;
    std::string _filepath;
};