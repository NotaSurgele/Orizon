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
#include "System.hpp"
#include "Core.hpp"

void loadScript(sol::state *state, const std::string& path)
{
    auto res = state->script_file(path);

    if (!res.valid()) {
        std::cerr << "[SCRIPT] error cannot import script " << path << std::endl;
        return;
    }
    std::cout << "[SCRIPT] successfully import script " << path << std::endl;
}

Script::Script(Entity *e, const std::string& scriptPath) :  _self(e),
                                                            _filepath(scriptPath)
{
    create(scriptPath);
}

void Script::create(const std::string& scriptPath, bool insert)
{
    _state = new sol::state();
    _state->open_libraries(sol::lib::base
            , sol::lib::math, sol::lib::string, sol::lib::table,
            sol::lib::package, sol::lib::io, sol::lib::os, sol::lib::debug);
    registerBaseTypes();
    registerComponentsType();
    registerEntityFunction();
    (*_state)["_self"] = _self;
    (*_state)["_state"] = _state;
    (*_state)["Utils"] = Utils();
    (*_state)["ResourceManager"] = Core::RessourceManager();
    (*_state)["DRAW"] = sol::overload(
            [](Core* core, Drawable *drawable) {
                return core->CoreDraw(drawable);
            },
            [] (Core* core, const sf::Drawable& drawable) {
                return core->CoreDraw(drawable);
            }
    );
    _state->set_function("Import", &loadScript);
    auto result = _state->script_file(scriptPath);

    if (!result.valid()) {
        sol::error res = result;
        std::cerr << "Error executing lua script " << res.what() << std::endl;
    } else {
        if (insert) {
            R_ADD_SCRIPT(scriptPath);
            System::__registerScriptedEntity(_self);
            std::cout << "[SCRIPT] Successfully imported script " << scriptPath << std::endl;
        }
    }
}

void Script::setScript(const std::string& filePath)
{
    _filepath = filePath;
    reload();
}

void Script::registerInputSystem()
{
    sol::usertype<Input> inputType = _state->new_usertype<Input>(
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
        "isButtonDown", &Input::isButtonDown,
        "isAnyButtonPressed", &Input::isAnyButtonPressed,
        "isButtonReleased", &Input::isButtonReleased,
        "isActionButtonDown", &Input::isActionButtonDown,
        "isActionButtonReleased", &Input::isActionButtonReleased,
        "isActionButtonPressed", &Input::isActionButtonPressed
    );
    (*_state)["Input"] = Input();
}

void Script::registerVectorType()
{
    // float
    sol::usertype<sf::Vector2f> vf = _state->new_usertype<sf::Vector2f>(
            "Vector2f", sol::constructors<sf::Vector2f(), sf::Vector2f(float, float)>()
    );
    vf["x"] = &sf::Vector2f::x;
    vf["y"] = &sf::Vector2f::y;

    // int
    sol::usertype<sf::Vector2i> vi = _state->new_usertype<sf::Vector2i>(
            "Vector2i", sol::constructors<sf::Vector2i(), sf::Vector2i(int, int)>()
    );
    vi["x"] = &sf::Vector2i::x;
    vi["y"] = &sf::Vector2i::y;

    //double
    sol::usertype<sf::Vector2<double>> vd = _state->new_usertype<sf::Vector2<double>>(
            "Vector2d", sol::constructors<sf::Vector2<double>(), sf::Vector2<double>(double, double)>()
    );
    vd["x"] = &sf::Vector2<double>::x;
    vd["y"] = &sf::Vector2<double>::y;

    // unsigned int
    sol::usertype<sf::Vector2u> vu = _state->new_usertype<sf::Vector2u>(
            "Vector2u", sol::constructors<sf::Vector2u(), sf::Vector2u(uint, uint)>()
    );
    vu["x"] = &sf::Vector2u::x;
    vu["y"] = &sf::Vector2u::y;
}

void Script::registerColorType()
{
    _state->new_usertype<sf::Color>(
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
    _state->new_usertype<sf::FloatRect>(
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
    _state->new_usertype<sf::IntRect>(
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

void Script::registerSystemType()
{
    _state->new_usertype<System>(
            "System", sol::constructors<System()>(),
            "pushEntity", &System::forceUpdate
    );
    (*_state)["system"] = System();
}

void Script::registerTileMap()
{
    _state->new_usertype<TileMap>(
            "TileMap", sol::constructors<TileMap(float, float, int, int, int, int)>(),
            "emplaceEntity", &TileMap::emplaceEntity,
            "contain", sol::overload(
                    [] (TileMap *map, Entity *e) {
                        return map->contain(e);
                    },
                    [] (TileMap *map, const float& x, const float& y) {
                        return map->contain(x, y);
                    }
            ),
            "isInside", &TileMap::isInside,
            "removeEntity", sol::overload(
                    [] (TileMap *map, Entity *e) {
                        return map->removeEntity(e);
                    },
                    [] (TileMap *map, const float& x, const float& y) {
                        return map->removeEntity(x, y);
                    }
            ),
            "isRender", &TileMap::isRender,
            "render", &TileMap::render,
            "hide", &TileMap::hide,
            "destroy", &TileMap::destroy,
            "getAllEntities", &TileMap::getAllEntities,
            "getLayerInfo", &TileMap::getLayerInfo
    );
}

void Script::registerUtilsType()
{
    _state->new_usertype<Utils>(
            "Utils", sol::constructors<Utils()>(),
            "readFile", &Utils::readFile,
            "writeFile", &Utils::writeFile);
}

void Script::registerResourceManager()
{
    _state->new_usertype<RessourcesManager>(
        "ResourceManager",
        "R_ADD_TILE", &RessourcesManager::loadTileFromSpriteSheet,
        "R_GET_RESSOURCE", sol::overload(
            [] (RessourcesManager& rm, const std::string& resourceName) {
                return rm.getRessource<sf::Texture>(resourceName);
            },
            [] (RessourcesManager& rm, const std::string& resourceName) {
                return rm.getRessource<sf::SoundBuffer>(resourceName);
            }
        )
    );
}

void Script::registerDrawableType()
{
    _state->new_usertype<sf::Drawable>("sf::Drawable");
    _state->new_usertype<Drawable>("Drawable", "draw", &Drawable::draw);
}

void Script::registerCoreType()
{
    _state->new_usertype<Core>(
        "Core",
        "draw", sol::overload(
            [] (Core *core, BoxCollider *drawable) {
                return core->CoreDraw(drawable);
            },
            [] (Core *core, const sf::Drawable& drawable) {
                return core->CoreDraw(drawable);
            }
        )
   );
    (*_state)["Core"] = Core::instance;
}

void Script::registerBaseTypes()
{
    registerInputSystem();
    registerVectorType();
    registerColorType();
    registerRectType();
    registerTileMap();
    registerUtilsType();
    registerResourceManager();
    registerSystemType();
    registerDrawableType();
    registerCoreType();
}

void Script::registerTransform2DComponent()
{
    sol::usertype<Transform2D> t = _state->new_usertype<Transform2D>(
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
    _state->new_usertype<Animator>(
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
    auto box = _state->new_usertype<BoxCollider>(
            "BoxCollider", sol::constructors<BoxCollider(Entity *, sf::Vector2f, sf::Vector2f),
                    BoxCollider(Entity *, sf::Vector2f, sf::Vector2f, int)>(),
            "onCollision", &BoxCollider::onCollision,
            "entity", &BoxCollider::entity,
            "setType", &BoxCollider::setType
    );
    _state->new_enum("BoxColliderType",
                     "STATIC", BoxCollider::Type::STATIC,
                     "DYNAMIC", BoxCollider::Type::DYNAMIC,
                     "NUL", BoxCollider::Type::NUL
    );
}

void Script::registerGravityComponent()
{
    _state->new_usertype<Gravity>(
        "Gravity", sol::constructors<Gravity(Entity *), Gravity(Entity *, double)>(),
        "force", &Gravity::force,
        "destroy", &Gravity::destroy
    );
}

void Script::registerLayerComponent()
{
    _state->new_usertype<Layer>(
        "Layer", sol::constructors<Layer(Entity *, std::size_t)>(),
        "set", &Layer::set,
        "value", &Layer::value,
        "destroy", &Layer::destroy
    );
}

void Script::registerLightComponent()
{
    _state->new_usertype<Light>(
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
    _state->new_usertype<OrizonMusic>(
        "OrizonMusic", sol::constructors<OrizonMusic(Entity *, const std::string&)>(),
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
    _state->new_usertype<Sound>(
        "Sound", sol::constructors<Sound(Entity *, const std::string&)>(),
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
    _state->new_usertype<Sprite>(
        "Sprite", sol::constructors<
                            Sprite(Entity *, sf::Texture, float, float),
                            Sprite(Entity *, sf::Texture),
                            Sprite(Entity *, std::string, float, float),
                            Sprite(sf::Texture texture)>(),
        "setScale", &Sprite::setScale,
        "setTexture", sol::overload(
                [] (Sprite *sprite, const std::string& texturePath) {
                    return sprite->setTexture(texturePath);
                },
                [] (Sprite *sprite, sf::Texture& texture) {
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
    _state->new_usertype<Velocity>(
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
    _state->new_usertype<Tag>(
        "Tag", sol::constructors<Tag(Entity *, std::string)>(),
        "value", &Tag::value,
        "equals", &Tag::equals,
        "destroy", &Tag::destroy
    );
}

void Script::registerViewComponent()
{
    _state->new_usertype<View>(
        "View", sol::constructors<View(Entity *, float, float, float, float, bool)>(),
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

void Script::registerScriptComponent()
{
    _state->new_usertype<Script>(
            "Script", sol::constructors<Script(Entity *, const std::string&)>(),
            "call", sol::overload(&Script::call<Entity *>,
                                  &Script::call<int>,
                                  &Script::call<std::string>,
                                  &Script::call<float>,
                                  &Script::call<double>,
                                  &Script::call<bool>,
                                  &Script::call<sf::Vector2f>,
                                  &Script::call<sf::Vector2i>,
                                  &Script::call<sf::Vector2u>,
                                  &Script::call<sf::Color>,
                                  &Script::call<sf::FloatRect>,
                                  &Script::call<sf::IntRect>)
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
    registerScriptComponent();
    registerViewComponent();
}

void Script::registerEntityFunction()
{
    sol::usertype<Entity> entityType = _state->new_usertype<Entity>(
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
                    [](Entity *entity, const std::string& name) {
                        return entity->addComponent<OrizonMusic>(name);
                    }
            ),
            "addComponentSound", sol::overload(
                    [](Entity *entity, const std::string& name) {
                        return entity->addComponent<Sound>(name);
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
                        return entity->addComponent<Tag>(std::move(tagName));
                    }
            ),
            "addComponentView", sol::overload(
                    [](Entity *entity, float x, float y, float w, float h, bool follow=false) {
                        return entity->addComponent<View>(x, y, w, h, follow);
                    }
            ),
            "addComponentScript", sol::overload(
                    [] (Entity *e, const std::string& path) {
                        return e->addComponent<Script>(path);
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
    entityType["getComponentScript"] = &Entity::getComponent<Script>;
}

void Script::reload()
{
    _state->collect_gc();
    _state->collect_garbage();
    _state->stack_clear();
    delete _state;
    create(_filepath, false);
    _start = false;
}

void Script::start()
{
    try {
        if (_start)
            return;
        (*_state)["_self"] = _self;
        sol::function start = (*_state)["Start"];
        start();
        _start = true;
    } catch (sol::error& error) {
        std::cerr << error.what() << std::endl;
    }
}

void Script::update()
{
    try {
        (*_state)["deltaTime"] = Time::deltaTime;
        sol::function update = (*_state)["Update"];
        update();
    } catch (sol::error& error) {
        std::cerr << error.what() << std::endl;
    }
}

void Script::handleTypeTransformation(std::vector<sol::object> &modifiedArgs, int i)
{
    if (modifiedArgs[i].is<sol::userdata>()) {
        sol::userdata ud = modifiedArgs[i].as<sol::userdata>();

        for (auto& it : typesArray) {
            auto res = it(ud);
            if (res != sol::nil) {
                modifiedArgs[i] = res;
                return;
            }
        }
    }
}

