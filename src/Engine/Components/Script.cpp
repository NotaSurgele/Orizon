#include "Script.hpp"
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

Script::Script(Entity *e, const std::string& scriptPath) :  _self(e),
                                                            _filepath(scriptPath)
{
    _state.open_libraries(sol::lib::base);

    // register entity type inside lua script
    registerBaseTypes();
    registerComponentsType();
    registerEntityFunction();
    // registered attached entity
    _state["self"] = e;
    // set Getter
    _state.script_file(scriptPath);
}

void Script::registerInputSystem()
{
    sol::usertype<Input> inputType = _state.new_usertype<Input>(
        "Input",  // Name of the type in Lua

        // Constructors
        sol::constructors<Input()>(),
        // Member functions
        "isActionKeyDown", &Input::isActionKeyDown,
        "isActionPressed", &Input::isActionPressed
    );
    _state["Input"] = Input();
}

void Script::registerVectorType()
{
   // float
    sol::usertype<sf::Vector2f> vf = _state.new_usertype<sf::Vector2f>(
        "Vector2f", sol::constructors<sf::Vector2f(), sf::Vector2f(float, float)>()
    );
    vf["x"] = &sf::Vector2f::x;
    vf["y"] = &sf::Vector2f::y;

     // int
    sol::usertype<sf::Vector2i> vi = _state.new_usertype<sf::Vector2i>(
            "Vector2i", sol::constructors<sf::Vector2i(), sf::Vector2i(int, int)>()
    );
    vi["x"] = &sf::Vector2i::x;
    vi["y"] = &sf::Vector2i::y;

    //double
    sol::usertype<sf::Vector2<double>> vd = _state.new_usertype<sf::Vector2<double>>(
            "Vector2d", sol::constructors<sf::Vector2<double>(), sf::Vector2<double>(double, double)>()
    );
    vd["x"] = &sf::Vector2<double>::x;
    vd["y"] = &sf::Vector2<double>::y;

    // unsigned int
    sol::usertype<sf::Vector2u> vu = _state.new_usertype<sf::Vector2u>(
            "Vector2u", sol::constructors<sf::Vector2u(), sf::Vector2u(uint, uint)>()
    );
    vu["x"] = &sf::Vector2u::x;
    vu["y"] = &sf::Vector2u::y;
}

void Script::registerBaseTypes()
{
    registerInputSystem();
    registerVectorType();
}

void Script::registerTransform2DComponent()
{
    sol::usertype<Transform2D> t = _state.new_usertype<Transform2D>(
            "Transform2D",
            "position", &Transform2D::position
    );
}

void Script::registerComponentsType()
{
    registerTransform2DComponent();
}

void Script::registerEntityFunction()
{
    sol::usertype<Entity> entityType = _state.new_usertype<Entity>(
            "Entity", sol::constructors<Entity()>(),
            "addComponentTransform2D",sol::overload(
                    [](Entity *entity, float a, float b, float c, float d) {
                        return entity->addComponent<Transform2D>(a, b, c, d);
                    },
                    [](Entity *entity, float x, float y) {
                        return entity->addComponent<Transform2D>(x, y);
                    },
                    [](Entity *entity) {
                        return entity->addComponent<Transform2D>();
                    }
            ),
            "addComponentAnimator", sol::overload(
                    [](Entity *entity, float a, float b, float c, float d) {
                        return entity->addComponent<Animator>();
                    }
            ),
            "addComponentBoxCollider", sol::overload(
                    [](Entity *entity, sf::Vector2f position, sf::Vector2f size) {
                        return entity->addComponent<BoxCollider>(position, size);
                    },
                    [](Entity *entity, sf::Vector2f position, sf::Vector2f size, int range) {
                        return entity->addComponent<BoxCollider>(position, size, range);
                    }
            ),
            "addComponentGravity", sol::overload(
                    [](Entity *entity, double value) {
                        return entity->addComponent<Gravity>(value);
                    },
                    [](Entity *entity) {
                        return entity->addComponent<Gravity>();
                    }
            ),
            "addComponentLayer", sol::overload(
                    [](Entity *entity, std::size_t layer) {
                        return entity->addComponent<Layer>(layer);
                    }
            ),
            "addComponentLight", sol::overload(
                    [](Entity *entity, float emission, float intensity) {
                        return entity->addComponent<Light>(emission, intensity);
                    },
                    [](Entity *entity, float emission, Sprite *sprite, float intensity) {
                        return entity->addComponent<Light>(emission, sprite, intensity);
                    },
                    [](Entity *entity, float emission, Sprite *sprite) {
                        return entity->addComponent<Light>(emission, sprite);
                    },
                    [](Entity *entity, float emission) {
                        return entity->addComponent<Light>(emission);
                    }
            ),
            "addComponentOrizonMusic", sol::overload(
                    [](Entity *entity) {
                        return entity->addComponent<OrizonMusic>();
                    }
            ),
            "addComponentSound", sol::overload(
                    [](Entity *entity) {
                        return entity->addComponent<Sound>();
                    }
            ),
            "addComponentSprite", sol::overload(
                    [](Entity *entity, sf::Texture texture, float width=1, float height=1) {
                        return entity->addComponent<Sprite>(texture, width, height);
                    },
                    [](Entity *entity, sf::Texture texture) {
                        return entity->addComponent<Sprite>(texture);
                    },
                    [](Entity *entity, std::string texturePath, float width=1, float height=1) {
                        return entity->addComponent<Sprite>(texturePath, width, height);
                    }
            ),
            "addComponentVelocity", sol::overload(
                    [](Entity *entity) {
                        entity->addComponent<Velocity>();
                    }
            ),
             "addComponentTag", sol::overload(
                    [](Entity *entity, std::string tagName) {
                        return entity->addComponent<Tag>(tagName);
                    }
            ),
            "addComponentView", sol::overload(
                    [](Entity *entity, float x, float y, float w, float h, bool follow=false) {
                        return entity->addComponent<View>(x, y, w, h, follow);
                    }
            )
    );
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
    entityType["getComponentVelocity"] = &Entity::getComponent<Velocity>;
    entityType["getComponentView"] = &Entity::getComponent<View>;
}

void Script::reload()
{
    _state.script_file(_filepath);
    _start = false;
}

void Script::start()
{
    if (_start)
        return;
    sol::function start = _state["Start"];
    start();
    _start = true;
}

void Script::update()
{
    _state["deltaTime"] = Time::deltaTime;
    sol::function update = _state["Update"];
    update();
}