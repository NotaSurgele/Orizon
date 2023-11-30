#include "Script.hpp"

Script::Script(Entity *e, const std::string& scriptPath) : _self(e)
{
    _state.open_libraries(sol::lib::base);
    _instance = _state.load_file(scriptPath);

    // register entity type inside lua script
    sol::usertype<Entity> entityType = _state.new_usertype<Entity>(
            "Entity", sol::constructors<Entity()>(),
            // Add component functions with first string and static_cast for each component
            "addComponentTransform2D", sol::overload(
                    static_cast<Transform2D* (Entity::*)(float, float, float, float, float)>(&Entity::addComponent<Transform2D>),
                    static_cast<Transform2D* (Entity::*)()>(&Entity::addComponent<Transform2D>),
                    static_cast<Transform2D* (Entity::*)(float, float, float, float, float, float, float)>(&Entity::addComponent<Transform2D>)
            ),

            "addComponentAnimator", sol::overload(
                    static_cast<Animator* (Entity::*)()>(&Entity::addComponent<Animator>)
            ),

            "addComponentBoxCollider", sol::overload(
                    static_cast<BoxCollider* (Entity::*)(sf::Vector2f, sf::Vector2f)>(&Entity::addComponent<BoxCollider, sf::Vector2f, sf::Vector2f>),
                    static_cast<BoxCollider* (Entity::*)(sf::Vector2f, sf::Vector2f, int)>(&Entity::addComponent<BoxCollider, sf::Vector2f, sf::Vector2f, int>)
            ),

            "addComponentGravity", sol::overload(
                    static_cast<Gravity* (Entity::*)(double)>(&Entity::addComponent<Gravity, double>),
                    static_cast<Gravity* (Entity::*)()>(&Entity::addComponent<Gravity>)
            ),

            "addComponentLayer", sol::overload(
                    static_cast<Layer* (Entity::*)(std::size_t)>(&Entity::addComponent<Layer, std::size_t>)
            ),

            "addComponentLight", sol::overload(
                    static_cast<Light* (Entity::*)(float, float)>(&Entity::addComponent<Light, float, float>),
                    static_cast<Light* (Entity::*)(float)>(&Entity::addComponent<Light, float>),
                    static_cast<Light* (Entity::*)(float, Sprite*, float)>(&Entity::addComponent<Light, float, Sprite*, float>),
                    static_cast<Light* (Entity::*)(float, Sprite*)>(&Entity::addComponent<Light, float, Sprite*>)
            ),

            "addComponentOrizonMusic", sol::overload(
                    static_cast<OrizonMusic* (Entity::*)()>(&Entity::addComponent<OrizonMusic>)
            ),

            "addComponentSound", sol::overload(
                    static_cast<Sound* (Entity::*)()>(&Entity::addComponent<Sound>)
            ),

            "addComponentSprite", sol::overload(
                    static_cast<Sprite* (Entity::*)(sf::Texture, float, float)>(&Entity::addComponent<Sprite, sf::Texture, float, float>),
                    static_cast<Sprite* (Entity::*)(sf::Texture)>(&Entity::addComponent<Sprite, sf::Texture>)
            ),

            "addComponentTag", sol::overload(
                    static_cast<Tag* (Entity::*)(std::string)>(&Entity::addComponent<Tag, std::string>)
            ),

            "addComponentVelocity", sol::overload(
                    static_cast<Velocity<float>* (Entity::*)()>(&Entity::addComponent<Velocity<float>>)
            ),

            "addComponentView", sol::overload(
                    static_cast<View* (Entity::*)(float, float, float, float, bool)>(&Entity::addComponent<View, float, float, float, float, bool>),
                    static_cast<View* (Entity::*)(float, float, float, float)>(&Entity::addComponent<View, float, float, float, float>)
            )
    );

    // registered attached entity
    _state["self"] = e;
    // set Getter
    entityType["getComponentTransform2D"] = &Entity::getComponent<Transform2D>;
    entityType["getComponentTransform2D"] = &Entity::getComponent<Transform2D>;
    entityType["getComponentAnimator"] = &Entity::getComponent<Animator>;
    entityType["getComponentBoxCollider"] = &Entity::getComponent<BoxCollider>;
    entityType["getComponentGravity"] = &Entity::getComponent<Gravity>;
    entityType["getComponentLayer"] = &Entity::getComponent<Layer>;
    entityType["getComponentLight"] = &Entity::getComponent<Light>;
    entityType["getComponentMusic"] = &Entity::getComponent<OrizonMusic>;
    entityType["getComponentSound"] = &Entity::getComponent<Sound>;
    entityType["getComponentSprite"] = &Entity::getComponent<Sprite>;
    entityType["getComponentTag"] = &Entity::getComponent<Tag>;
    entityType["getComponentVelocityF"] = &Entity::getComponent<Velocity<float>>;
    entityType["getComponentView"] = &Entity::getComponent<View>;
}

void Script::removeEntityComponent(Entity *e, const std::string &type)
{
    ComponentType cType = _componentType[type];

    switch (cType) {
        case TRANSFORM2D: e->removeComponent<Transform2D>();
        case ANIMATOR: e->removeComponent<Animator>();
        case BOXCOLLIDER: e->removeComponent<BoxCollider>();
        case GRAVITY: e->removeComponent<Gravity>();
        case LAYER: e->removeComponent<Layer>();
        case LIGHT: e->removeComponent<Light>();
        case ORIZONMUSIC: e->removeComponent<OrizonMusic>();
        case SOUND: e->removeComponent<Sound>();
        case SPRITE: e->removeComponent<Sprite>();
        case TAG: e->removeComponent<Tag>();
        case VELOCITY: e->removeComponent<Velocity<float>>();
        case VIEW: e->removeComponent<View>();
        default: std::cerr << "Component type " << type << " does not exist " << std::endl; break;
    }
}

void Script::addComponentToEntity(Entity *e, const std::string& type)
{
    ComponentType cType = _componentType[type];

    switch (cType) {
        case TRANSFORM2D: e->addComponent<Transform2D>();
        case ANIMATOR: e->addComponent<Transform2D>();
        case BOXCOLLIDER: e->addComponent<Transform2D>();
        case GRAVITY: e->addComponent<Transform2D>();
        case LAYER: e->addComponent<Transform2D>();
        case LIGHT: e->addComponent<Transform2D>();
        case ORIZONMUSIC: e->addComponent<Transform2D>();
        case SOUND: e->addComponent<Transform2D>();
        case SPRITE: e->addComponent<Transform2D>();
        case TAG: e->addComponent<Transform2D>();
        case VELOCITY: e->addComponent<Transform2D>();
        case VIEW: e->addComponent<Transform2D>();
        default : std::cerr << "Component type " << type << " does not exist !" << std::endl; break;
    };
}

void Script::call()
{
    _instance();
}