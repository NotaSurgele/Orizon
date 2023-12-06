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
        "Input", sol::constructors<Input()>(),
        // Action key
        "isActionKeyDown", &Input::isActionKeyDown,
        "isActionKeyReleased", &Input::isActionKeyReleased,
        "isActionKeyPressed", &Input::isActionKeyPressed,
        "isKeyDown", &Input::isKeyDown,
        "isAnyKeyPressed", &Input::isAnyKeyPressed,
        "isKeyReleased", &Input::isKeyReleased,
        "isKeyPressed", &Input::isKeyPressed,
        "isButtonPressed", &Input::isButtonPressed,
        "isAnyButtonPressed", &Input::isAnyButtonPressed,
        "isButtonReleased", &Input::isButtonReleased,
        "isActionButtonDown", &Input::isActionButtonDown,
        "isActionButtonReleased", &Input::isActionButtonReleased,
        "isActionButtonPressed", &Input::isActionButtonPressed
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

void Script::registerColorType()
{
    _state.new_usertype<sf::Color>(
        "Color", sol::constructors<sf::Color(uint8_t, uint8_t, uint8_t, uint8_t),
                                            sf::Color()>(),
            "r", &sf::Color::r,
            "g", &sf::Color::g,
            "b", &sf::Color::b,
            "a", &sf::Color::a
    );
}

void Script::registerRectType()
{
    _state.new_usertype<sf::FloatRect>(
        "FloatRect", sol::constructors<sf::FloatRect(),
                                        sf::FloatRect(float, float, float, float),
                                        sf::FloatRect(sf::Vector2f, sf::Vector2f)>(),
        "x", &sf::FloatRect::left,
        "y", &sf::FloatRect::top,
        "width", &sf::FloatRect::width,
        "height", &sf::FloatRect::height,
        "contains", sol::overload(
            [] (sf::FloatRect& rect, float x, float y) {
                return rect.contains(x, y);
            },
            [] (sf::FloatRect& rect, const sf::Vector2f& position) {
                return rect.contains(position);
            }
        ),
        "intersects", sol::overload(
            [] (sf::FloatRect& rect, sf::FloatRect& rect2) {
                return rect.intersects(rect2);
            }
        ),
        "getPosition", &sf::FloatRect::getPosition,
        "getSize", &sf::FloatRect::getSize
    );
    _state.new_usertype<sf::IntRect>(
            "FloatRect", sol::constructors<sf::IntRect(),
                    sf::IntRect(int, int, int, int),
                    sf::IntRect(sf::Vector2i, sf::Vector2i)>(),
            "x", &sf::IntRect::left,
            "y", &sf::IntRect::top,
            "width", &sf::IntRect::width,
            "height", &sf::IntRect::height,
            "contains", sol::overload(
                    [] (sf::IntRect& rect, int x, int y) {
                        return rect.contains(x, y);
                    },
                    [] (sf::IntRect& rect, sf::Vector2i& position) {
                        return rect.contains(position);
                    }
            ),
            "intersects", sol::overload(
                    [] (sf::IntRect& rect, sf::IntRect& rect2) {
                        return rect.intersects(rect2);
                    }
            ),
            "getPosition", &sf::IntRect::getPosition,
            "getSize", &sf::IntRect::getSize
    );
}

void Script::registerBaseTypes()
{
    registerInputSystem();
    registerVectorType();
    registerColorType();
    registerRectType();
}

void Script::registerTransform2DComponent()
{
    sol::usertype<Transform2D> t = _state.new_usertype<Transform2D>(
        "Transform2D", sol::constructors<Transform2D(), Transform2D(float, float, float, float)>(),
        "position", &Transform2D::position,
        "rotation", &Transform2D::rotation,
        "size", &Transform2D::scale,
        "zero", &Transform2D::zero,
        "destroy", &Transform2D::destroy
    );
}

void Script::registerAnimatorComponent()
{
    _state.new_usertype<Animator>(
        "Animator", sol::constructors<Animator(Entity *)>(),
        "newAnimation", &Animator::newAnimation,
        "playAnimation", &Animator::playAnimation,
        "reset", &Animator::reset,
        "resetCurrentAnimation", &Animator::resetCurrentAnimation,
        "getAnimationsFrames", &Animator::getAnimationFrames,
        "destroy", &Animator::destroy,
        "currentAnimation", &Animator::currentAnimation
    );
}

void Script::registerBoxColliderComponent()
{
    _state.new_usertype<BoxCollider>(
    "BoxCollider", sol::constructors<BoxCollider(Entity *, sf::Vector2f, sf::Vector2f),
                                                    BoxCollider(Entity *, sf::Vector2f, sf::Vector2f, int)>(),
        "onCollision", &BoxCollider::onCollision,
        "entity", &BoxCollider::entity
    );
}

void Script::registerGravityComponent()
{
    _state.new_usertype<Gravity>(
    "Gravity", sol::constructors<Gravity(Entity *), Gravity(Entity *, double)>(),
        "force", &Gravity::force,
        "destroy", &Gravity::destroy
    );
}

void Script::registerLayerComponent()
{
    _state.new_usertype<Layer>(
        "Layer", sol::constructors<Layer(Entity *, std::size_t)>(),
        "set", &Layer::set,
        "value", &Layer::value,
        "destroy", &Layer::destroy
    );
}

void Script::registerLightComponent()
{
    _state.new_usertype<Light>(
    "Light", sol::constructors<
                                Light(Entity *, float, float),
                                Light(Entity *, float, Sprite *, float),
                                Light(Entity *, float, Sprite *),
                                Light(Entity *, float)>(),
        "setColor", &Light::setColor,
        "setEmission", &Light::setEmission,
        "getEmission", &Light::getEmission,
        "destroy", &Light::destroy,
        "loadColorFromIntensity", &Light::loadColorFromIntensity
    );
}

void Script::registerOrizonMusicComponent()
{
    _state.new_usertype<OrizonMusic>(
        "OrizonMusic", sol::constructors<OrizonMusic(Entity *)>(),
        "destroy", &OrizonMusic::destroy,
        "load", &OrizonMusic::load,
        "loadFromName", &OrizonMusic::loadFromName,
        "stop", &OrizonMusic::stop,
        "play", &OrizonMusic::play,
        "pause", &OrizonMusic::pause,
        "isLoop", &OrizonMusic::isLoop,
        "getDuration", &OrizonMusic::getDuration,
        "getVolume", &OrizonMusic::getVolume,
        "setLoop", &OrizonMusic::setLoop
    );
}

void Script::registerSoundComponent()
{
    _state.new_usertype<Sound>(
        "Sound", sol::constructors<Sound(Entity *)>(),
        "load", &Sound::load,
        "loadFromName", &Sound::loadFromName,
        "reload", &Sound::reload,
        "loadFromName", &Sound::loadFromName,
        "stop", &Sound::stop,
        "play", &Sound::play,
        "pause", &Sound::pause,
        "isLoop", &Sound::isLoop,
        "getVolume", &Sound::getVolume,
        "destroy", &Sound::destroy,
        "setLoop", &Sound::setLoop
    );
}

void Script::registerSpriteComponent()
{
    _state.new_usertype<Sprite>(

        "Sprite", sol::constructors<Sprite(Entity *, sf::Texture, float, float),
                                            Sprite(Entity *, sf::Texture),
                                            Sprite(Entity *, std::string, float, float)>(),
        "setScale", &Sprite::setScale,
        "setTexture", sol::overload(
            [] (Sprite *sprite, const std::string& texturePath) {
                return sprite->setTexture(texturePath);
            },
            [] (Sprite *sprite, const sf::Texture& texture) {
                return sprite->setTexture(texture);
            }
        ),
        "setScale", &Sprite::setScale,
        "setColor", &Sprite::setColor,
        "setPosition", sol::overload(
            [] (Sprite *sprite, const sf::Vector2f& position) {
                return sprite->setPosition(position);
            },
            [] (Sprite *sprite, const float& x, const float& y) {
                return sprite->setPosition(x, y);
            }
        ),
        "setTransform", &Sprite::setTransform,
        "setTextureRect", &Sprite::setTextureRect,
        "setSprite", &Sprite::setSprite,
        "getPosition", &Sprite::getPosition,
        "getTexture", &Sprite::getTexture,
        "getScale", &Sprite::getScale,
        "getColor", &Sprite::getColor,
        "getSprite", &Sprite::getSprite,
        "destroy", &Sprite::destroy
    );
}

void Script::registerVelocityComponent()
{
    _state.new_usertype<Velocity>(
        "Velocity", sol::constructors<Velocity(Entity *)>(),
        "getX", &Velocity::getX,
        "getY", &Velocity::getY,
        "setX", &Velocity::setX,
        "setY", &Velocity::setY,
        "values", &Velocity::values,
        "destroy", &Velocity::destroy
    );
}

void Script::registerTagComponent()
{
    _state.new_usertype<Tag>(
        "Tag", sol::constructors<Tag(Entity *, std::string)>(),
        "value", &Tag::value,
        "equals", &Tag::equals,
        "destroy", &Tag::destroy
    );
}

void Script::registerViewComponent()
{
    _state.new_usertype<View>(
        "View", sol::constructors<View(Entity *, float, float, float, float, bool)>(

                ),
        "setViewport", &View::setViewport,
        "setSize", sol::overload(
            [] (View *view, const sf::Vector2f& size) {
                return view->setSize(size);
            },
            [] (View *view, const float& w, const float& h) {
                return view->setSize(w, h);
            }
        ),
        "setCenter", sol::overload(
            [] (View *view, const sf::Vector2f& position) {
                return view->setCenter(position);
            },
            [] (View *view, const float& x, const float& y) {
                return view->setCenter(x, y);
            }
        ),
        "setRotation", &View::setRotation,
        "getSize", &View::getSize,
        "getCenter", &View::getCenter,
        "getTransform",  &View::getTransform,
        "getInverseTransform", &View::getInverseTransform,
        "getRotation", &View::getRotation
    );
}

void Script::registerComponentsType()
{
    registerTransform2DComponent();
    registerAnimatorComponent();
    registerBoxColliderComponent();
    registerGravityComponent();
    registerLayerComponent();
    registerLightComponent();
    registerOrizonMusicComponent();
    registerSoundComponent();
    registerSpriteComponent();
    registerVelocityComponent();
    registerTagComponent();
    registerViewComponent();
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
                    [](Entity *entity) {
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
            ),
            "destroy", &Entity::destroy
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