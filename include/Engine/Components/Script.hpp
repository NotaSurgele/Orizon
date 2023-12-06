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
    void registerAnimatorComponent();
    void registerBoxColliderComponent();
    void registerBaseTypes();

    // register component
    void registerTransform2DComponent();
    void registerComponentsType();
    void registerEntityFunction();

protected:
    bool _start = false;
    Entity *_self;
    sol::state _state;
    std::string _filepath;
};