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
#include "Transform2D.hpp"
#include "Velocity.hpp"
#include "View.hpp"
#include <unordered_map>
#include <string>

class Script : public IComponent {
public:
    Script(Entity *e, const std::string& scriptPath);
    Script() = delete;
    ~Script() = default;

    void call();

    void destroy() override {}

protected:
    void addComponentToEntity(Entity *e, const std::string& type);
    void removeEntityComponent(Entity *e, const std::string& type);

protected:
    enum ComponentType {
        TRANSFORM2D,
        ANIMATOR,
        BOXCOLLIDER,
        GRAVITY,
        LAYER,
        LIGHT,
        ORIZONMUSIC,
        SOUND,
        SPRITE,
        TAG,
        VELOCITY,
        VIEW,
        NONE,
    };

    Entity *_self;
    sol::state _state;
    sol::load_result _instance;
    std::unordered_map<std::string, ComponentType> _componentType = {
        { "Transform2D", ComponentType::TRANSFORM2D },
        { "Animator", ComponentType::ANIMATOR },
        { "BoxCollider", ComponentType::BOXCOLLIDER },
        { "Gravity", ComponentType::GRAVITY },
        { "Layer", ComponentType::LAYER },
        { "Light", ComponentType::LIGHT },
        { "OrizonMusic", ComponentType::ORIZONMUSIC },
        { "Sound", ComponentType::SOUND },
        { "Sprite", ComponentType::SPRITE },
        { "Tag", ComponentType::TAG },
        { "Velocity", ComponentType::VELOCITY },
        { "View", ComponentType::VIEW },
        { "View", ComponentType::VIEW },
    };
};
