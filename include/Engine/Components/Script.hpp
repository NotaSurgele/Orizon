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

    void call();

    void destroy() override {}

    void reload();

private:

    // base types
    void registerIComponentType();
    void registerVectorType();
    void registerBaseTypes();

    // register component
    void registerTransform2DComponent();
    void registerComponentsType();
    void registerEntityFunction();

protected:
    Entity *_self;
    sol::state _state;
    sol::load_result _instance;
    std::string _filepath;
};
