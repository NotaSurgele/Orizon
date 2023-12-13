#pragma once
#include "IComponent.hpp"
#include "Entity.hpp"
#include "sol/sol.hpp"
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

private:

    // System
    void registerInputSystem();

    // base types
    void registerVectorType();
    void registerColorType();
    void registerRectType();
    void registerSystemType();
    void registerTileMap();
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
    void getState(sol::state *state, const std::string& table);


protected:
    bool _start = false;
    Entity *_self;
    sol::state _state;
    std::string _filepath;
};