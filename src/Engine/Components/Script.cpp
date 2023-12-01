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
    _instance = _state.load_file(scriptPath);
}

void Script::registerVectorType()
{
   // float
    sol::usertype<sf::Vector2f> vf = _state.new_usertype<sf::Vector2f>(
        "Vector2f", sol::constructors<sf::Vector2f(), sf::Vector2f(float, float)>()
    );
    vf["x"] = &sf::Vector2f::x;
    vf["y"] = &sf::Vector2f::y;

/*     // int
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
    vu["y"] = &sf::Vector2u::y;*/
}

void Script::registerBaseTypes()
{
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
                    [](Entity& entity, float a, float b, float c, float d) {
                        return entity.addComponent<Transform2D>(a, b, c, d);
                    }
            )
            /* "addComponentTransform2D", sol::overload(
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

             "addComponentVelocityF", sol::overload(
                 static_cast<Velocity<float>* (Entity::*)()>(&Entity::addComponent<Velocity<float>>)
             ),

             "addComponentView", sol::overload(
                 static_cast<View* (Entity::*)(float, float, float, float, bool)>(&Entity::addComponent<View, float, float, float, float, bool>),
                     static_cast<View* (Entity::*)(float, float, float, float)>(&Entity::addComponent<View, float, float, float, float>)
             )*/
    );
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

void Script::reload()
{
    _state.collect_garbage();
    _state.open_libraries(sol::lib::base);
    _instance = _state.load_file(_filepath);

    // register entity type inside lua script

    registerBaseTypes();
    registerComponentsType();
    registerEntityFunction();
}

void Script::call()
{
    _instance();
}