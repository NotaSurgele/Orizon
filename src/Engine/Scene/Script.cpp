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
#include "Canvas.hpp"
#include "Tag.hpp"
#include "Velocity.hpp"
#include "View.hpp"
#include "Input.hpp"
#include "Time.hpp"
#include "System.hpp"
#include "Core.hpp"
#include "Line.hpp"

void loadScript(sol::state *state, const std::string& path)
{
    auto res = state->script_file(path);

    if (!res.valid()) {
        std::cerr << "[SCRIPT] error cannot import script " << path << std::endl;
        return;
    }
    std::cout << "[SCRIPT] successfully import script " << path << std::endl;
}

Script::Script(const std::string& scriptPath) : _filepath(scriptPath)
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
    (*_state)["Utils"] = Utils();
    (*_state)["DRAW"] = sol::overload(
            [](Core* core, Drawable *drawable) {
                return core->CoreDraw(drawable);
            },
            [] (Core* core, const sf::Drawable& drawable) {
                return core->CoreDraw(drawable);
            }
    );
    _state->set_function("Import", &loadScript);

    try {
        auto result = _state->script_file(scriptPath);

        if (!result.valid()) {
            sol::error res = result;
            std::cerr << "[SCRIPT] Error executing lua script " << res.what() << std::endl;
        }
        std::cout << "[SCRIPT] Successfully imported scripted scene: " << scriptPath << "!" << std::endl;
    } catch (std::exception& err) {
        std::cerr << err.what() << std::endl;
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
    auto color = _state->new_usertype<sf::Color>(
            "Color", sol::constructors<sf::Color(uint8_t, uint8_t, uint8_t, uint8_t),
                    sf::Color()>(),
            "r", &sf::Color::r,
            "g", &sf::Color::g,
            "b", &sf::Color::b,
            "a", &sf::Color::a
    );
    (*_state)["Color"]["Red"] = &sf::Color::Red;
    (*_state)["Color"]["Black"] = &sf::Color::Black;
    (*_state)["Color"]["Blue"] = &sf::Color::Blue;
    (*_state)["Color"]["Cyan"] = &sf::Color::Cyan;
    (*_state)["Color"]["Green"] = &sf::Color::Green;
    (*_state)["Color"]["Magenta"] = &sf::Color::Magenta;
    (*_state)["Color"]["Transparent"] = &sf::Color::Transparent;
    (*_state)["Color"]["White"] = &sf::Color::White;
    (*_state)["Color"]["Yellow"] = &sf::Color::Yellow;
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
            "IntRect", sol::constructors<sf::IntRect(),
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
        "pushEntity", &System::forceUpdate,
        "getEntity", sol::overload(
            [](const std::string& signature) {
                return System::getEntity(signature);
            },
            [] (const std::size_t& id) {
                return System::getEntity(id);
            }
        ),
        "getLocalMousePosition", &System::getLocalMousePosition,
        "getGlobalMousePosition", &System::getGlobalMousePosition,
        "localToGlobalCoordinate", &System::localToGlobalCoordinate,
        "globalToLocalCoordinate", &System::globalToLocalCoordinate
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
    _state->new_usertype<ResourcesManager>(
    "ResourceManager",
    "addTile", &ResourcesManager::loadTileFromSpriteSheet,
        "getResource", sol::overload(
            [] (const std::string& resourceName) {
                return Core::resourceManager().getRessource<sf::Texture>(resourceName);
            },
            [] (const std::string& resourceName) {
                return Core::resourceManager().getRessource<sf::SoundBuffer>(resourceName);
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
            [] (Core *core, Sprite *sprite) {
                return core->CoreDraw(sprite);
            },
            [] (Core *core, const sf::Drawable& drawable) {
                return core->CoreDraw(drawable);
            }
        )
   );
    (*_state)["Core"] = Core::instance;
}

void Script::registerCanvasTypes()
{
    _state->new_enum("CoordType",
                     "WORLD", CanvasObject::CoordType::WORLD,
                     "LOCAL", CanvasObject::CoordType::LOCAL);
    _state->new_usertype<CanvasObject>(
        "CanvasObject",
        "coordType", &CanvasObject::coordType,
        "getOffset", &CanvasObject::getOffset,
        "setOffset", sol::overload(
            [] (CanvasObject *obj, const sf::Vector2f& offset) {
                return obj->setOffset(offset);
            },
            [] (CanvasObject *obj, const float& x, const float& y) {
                return obj->setOffset(x, y);
            }
        ),
        "getZ", &CanvasObject::getZ,
        "setZ", &CanvasObject::setZ,
        "setBasePosition", sol::overload(
            [](CanvasObject *obj, const sf::Vector2f& position) {
                return obj->setBasePosition(position);
            },
            [](CanvasObject *obj, const float& x, const float& y) {
                return obj->setBasePosition(x, y);
            }
        ),
        "getBasePosition", &CanvasObject::getBasePosition
    );

    _state->new_usertype<Text>(
    "Text", sol::constructors<Text(), Text(const std::string&, const sf::Font&, const std::size_t&)>(),
        "coordType", &Text::coordType,
        "getOffset", &Text::getOffset,
        "getZ", &Text::getZ,
        "setZ", &Text::setZ,
        "setOffset", sol::overload(
            [] (Text *obj, const sf::Vector2f& offset) {
                return obj->setOffset(offset);
            },
            [] (Text *obj, const float& x, const float& y) {
                return obj->setOffset(x, y);
            }
        ),
        "setBasePosition", sol::overload(
            [](Text *obj, const sf::Vector2f& position) {
                return obj->setBasePosition(position);
            },
            [](Text *obj, const float& x, const float& y) {
                return obj->setBasePosition(x, y);
            }
        ),
        "getBasePosition", &Text::getBasePosition
    );

    _state->new_usertype<Image>(
        "Image", sol::constructors<Image(sf::Texture&, const sf::Vector2f&,
                                         const sf::Vector2f& scale, const sf::Color& color)>(),
        "setPosition", sol::overload(
            [](Image *img, const float& x, const float& y) {
                return img->setPosition(x, y);
            },
            [](Image *img, const sf::Vector2f& position) {
                return img->setPosition(position);
            }
        ),
        "setColor", &Image::setColor,
        "getPosition", &Image::getPosition,
        "getTextureSize", &Image::getTextureSize,
        "setSize", &Image::setSize,
        "getImage", &Image::getImage,
        "getOffset", &Image::getOffset,
        "setOffset", sol::overload(
            [] (Image *obj, const sf::Vector2f& offset) {
                return obj->setOffset(offset);
            },
            [] (Image *obj, const float& x, const float& y) {
                return obj->setOffset(x, y);
            }
        ),
        "getZ", &Image::getZ,
        "setZ", &Image::setZ,
        "setBasePosition", sol::overload(
            [](Image *obj, const sf::Vector2f& position) {
                return obj->setBasePosition(position);
            },
            [](Image *obj, const float& x, const float& y) {
                return obj->setBasePosition(x, y);
            }
        ),
        "getBasePosition", &Image::getBasePosition,
        "coordType", &Image::coordType
    );

    _state->new_enum("States",
        "HOVERED", Button::States::HOVERED,
        "PRESSED", Button::States::PRESSED,
        "NOTHING", Button::States::NOTHING
    );
    _state->new_usertype<Button>(
        "Button", sol::constructors<Button(const sf::Vector2f&, const sf::Vector2f&, sf::Texture&)>(),
        "getSprite", &Button::getSprite,
        "setTexture", sol::overload(
            [](Button *button, sf::Texture& texture) {
                return button->setTexture(texture);
            },
            [](Button *button, sf::Texture& texture, const std::string& name) {
                return button->setTexture(texture, name);
            }
        ),
        "setColor", &Button::setColor,
        "setCallback", &Button::setCallback,
        "setText", sol::overload(
            [](Button *button, const std::string& content, const std::size_t& size) {
                return button->setText(content, size);
            },
            [](Button button, const std::string& content, const std::size_t& size, const sf::Color& color) {
                return button.setText(content, size, color);
            }
        ),
        "setPosition", sol::overload(
            [](Button *button, const float& x, const float& y) {
                return button->setPosition(x, y);
            },
            [](Button *button, const sf::Vector2f& position) {
                return button->setPosition(position);
            }
        ),
        "setScale", sol::overload(
            [](Button *button, const sf::Vector2f& size) {
                return button->setScale(size);
            },
            [](Button *button, const float& x, const float& y) {
                return button->setScale(x, y);
            }
        ),
        "clickable", &Button::clickable,
        "isHovered", &Button::isHovered,
        "isClicked", &Button::isClicked,
        "getPosition", &Button::getPosition,
        "getSize", &Button::getSize,
        "getTextureSize", &Button::getTextureSize,
        "getZ", &Button::getZ,
        "setZ", &Button::setZ,
        "coordType", &Button::coordType,
        "getOffset", &Button::getOffset,
        "setOffset", sol::overload(
            [] (Button *obj, const sf::Vector2f& offset) {
                return obj->setOffset(offset);
            },
            [] (Button *obj, const float& x, const float& y) {
                return obj->setOffset(x, y);
            }
        ),
        "text", &Button::text,
        "getBasePosition", &Button::getBasePosition,
        "setBasePosition", sol::overload(
            [](Button *obj, const sf::Vector2f& position) {
                return obj->setBasePosition(position);
            },
            [](Button *obj, const float& x, const float& y) {
                return obj->setBasePosition(x, y);
            }
        )
    );
}

void Script::registerLineType()
{
    _state->new_usertype<Line>(
        "Line", sol::constructors<Line(float, float, float, float, float, sf::Color),
        Line(float, float, float, float), Line(float, float, float, float, float)>(),
        "setColor", &Line::setColor,
        "setEndPoint", sol::overload(
            [](Line& line, float x2, float y2) {
                return line.setEndPoint(x2, y2);
            },
            [](Line& line, const sf::Vector2f& point) {
                return line.setEndPoint(point);
            }
        ),
        "setStartPoint", sol::overload(
            [] (Line& line, const sf::Vector2f& point) {
                return line.setStartPoint(point);
            },
            [] (Line& line, float x1, float y1) {
                return line.setStartPoint(x1, y1);
            }
        ),
        "setThickness", &Line::setThickness,
        "getStartPoint", &Line::getStartPoint,
        "getEndPoint", &Line::getEndPoint,
        "draw", &Line::draw
    );
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
    registerCanvasTypes();
    registerCoreType();
    registerLineType();
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
                Light(Entity *, Sprite *),
                Light(Entity *, Sprite *, float)>(),
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
        "getRotation", &Sprite::getRotation,
        "getSprite", &Sprite::getSprite,
        "getGlobalBounds", &Sprite::getGlobalBounds,
        "rotate", &Sprite::rotate,
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
    _state->new_usertype<Scene>(
        "Scene",
        "loadSceneFromFile", &Scene::loadSceneFromFile
    );
    _state->new_usertype<Script>(
            "Script",
            "loadSceneFromFile", &Script::loadSceneFromFile
    );
    _state->set_function("loadSceneFromFile", &Script::loadSceneFromFile);
}

void Script::registerCanvasComponent()
{
    _state->new_usertype<Canvas>(
        "Canvas", sol::constructors<Canvas(Entity *)>(),
        "addText", sol::overload(
            [] (Canvas *canvas, const std::string& content, const sf::Vector2f& pos, const std::size_t& size) {
                return canvas->addText(content, pos, size, false);
            }
        ),
        "addButton", sol::overload(
            [] (Canvas *canvas, const sf::Vector2f& pos, const sf::Vector2f& scale, const sf::Texture& text) {
                return canvas->addButton(pos, scale, text, false);
            }
        ),
        "addImage", sol::overload(
            [] (Canvas *canvas, sf::Texture texture, const sf::Vector2f& position, const sf::Vector2f& scale) {
                return canvas->addImage(texture, position, scale, false);
            }
        ),
        "removeObject", sol::overload(
        [](Canvas *canvas, Button *button) {
                return canvas->removeObject<Button *>(button);
            },

            [](Canvas *canvas, Text *text) {
                return canvas->removeObject<Text *>(text);
            },

            [] (Canvas *canvas, Image *img) {
                return canvas->removeObject<Image *>(img);
            }
        ),
        "getTexts", &Canvas::getTexts,
        "getButtons", &Canvas::getButtons,
        "getImages", &Canvas::getImages
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
    registerCanvasComponent();
    registerViewComponent();
}

void Script::registerEntityFunction()
{
    sol::usertype<Entity> entityType = _state->new_usertype<Entity>(
            "Entity", sol::constructors<Entity()>(),
            "addTransform2D",sol::overload(
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
            "addAnimator", sol::overload(
                    [](Entity *entity) {
                        return entity->addComponent<Animator>();
                    }
            ),
            "addBoxCollider", sol::overload(
                    [](Entity *entity, sf::Vector2f position, sf::Vector2f size) {
                        return entity->addComponent<BoxCollider>(position, size);
                    },
                    [](Entity *entity, sf::Vector2f position, sf::Vector2f size, int range) {
                        return entity->addComponent<BoxCollider>(position, size, range);
                    }
            ),
            "addGravity", sol::overload(
                    [](Entity *entity, double value) {
                        return entity->addComponent<Gravity>(value);
                    },
                    [](Entity *entity) {
                        return entity->addComponent<Gravity>();
                    }
            ),
            "addLight", sol::overload(
                    [](Entity *entity, Sprite *sprite, float intensity) {
                        return entity->addComponent<Light>(sprite, intensity);
                    },
                    [](Entity *entity, Sprite *sprite) {
                        return entity->addComponent<Light>(sprite);
                    }
            ),
            "addOrizonMusic", sol::overload(
                    [](Entity *entity, const std::string& name) {
                        return entity->addComponent<OrizonMusic>(name);
                    }
            ),
            "addSound", sol::overload(
                    [](Entity *entity, const std::string& name) {
                        return entity->addComponent<Sound>(name);
                    }
            ),
            "addSprite", sol::overload(
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
            "addVelocity", sol::overload(
                    [](Entity *entity) {
                        entity->addComponent<Velocity>();
                    }
            ),
            "addTag", sol::overload(
                    [](Entity *entity, std::string tagName) {
                        return entity->addComponent<Tag>(std::move(tagName));
                    }
            ),
            "addView", sol::overload(
                    [](Entity *entity, float x, float y, float w, float h, bool follow=false) {
                        return entity->addComponent<View>(x, y, w, h, follow);
                    }
            ),
            /*"addScript", sol::overload(
                    [] (Entity *e, const std::string& path) {
                        return e->addComponent<Script>(path);
                    }
            ),*/
            "addCanvas", sol::overload(
                [](Entity *e) {
                    return e->addComponent<Canvas>();
                }
            ),
            "destroy", &Entity::destroy
    );
    entityType["getTransform2D"] = &Entity::getComponent<Transform2D>;
    entityType["getAnimator"] = &Entity::getComponent<Animator>;
    entityType["getBoxCollider"] = &Entity::getComponent<BoxCollider>;
    entityType["getGravity"] = &Entity::getComponent<Gravity>;
    entityType["getLayer"] = &Entity::getComponent<Layer>;
    entityType["getLight"] = &Entity::getComponent<Light>;
    entityType["getMusic"] = &Entity::getComponent<OrizonMusic>;
    entityType["getSound"] = &Entity::getComponent<Sound>;
    entityType["getSprite"] = &Entity::getComponent<Sprite>;
    entityType["getTag"] = &Entity::getComponent<Tag>;
    entityType["getVelocity"] = &Entity::getComponent<Velocity>;
    entityType["getView"] = &Entity::getComponent<View>;
    /*
    entityType["getScript"] = &Entity::getComponent<Script>;
    */
    entityType["getCanvas"] = &Entity::getComponent<Canvas>;
}

void Script::reload()
{
    System::forceDestroy();
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
        //(*_state)["_self"] = _self;
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

void Script::destroy() {
    try {
        sol::function destroy = (*_state)["Destroy"];
        destroy();
    } catch (sol::error& error) {
        std::cerr << error.what() << std::endl;
    }
}

std::variant<Script *,Entity *, sf::FloatRect,sf::Vector2f,
sf::Vector2i,sf::Vector2u, sf::IntRect,sf::Color, sol::nil_t, sol::table,sol::object>
Script::getTableValue(const sol::object& res)
{
    auto ud = res.as<sol::userdata>();

    for (auto& func : typesArray) {
        auto res = func(ud);
        if (res != sol::nil) {
            return res;
        }
    }
    return res;
}

sol::table Script::deserializeTable(const sol::table &table)
{
    sol::table newTable = _state->create_table();

    newTable[sol::metatable_key] = table[sol::metatable_key];
    table.for_each([&](const sol::object& key, const sol::object& value) {
        if (value.is<sol::userdata>()) {
            newTable[key.as<std::string>()] = getTableValue(value);
            return;
        }

        if (value.is<sol::metatable>()) {
            newTable[key.as<std::string>()] = deserializeTable(value.as<sol::metatable>());
            return;
        }
        newTable[key.as<std::string>()] = value;
    });
    _state->set(newTable);
    return newTable;
}

std::variant<Script *, Entity *, sf::FloatRect, sf::Vector2f,
sf::Vector2i, sf::Vector2u, sf::IntRect, sf::Color, sol::nil_t, sol::table, sol::object>
Script::call(const std::string &function, sol::variadic_args args)
{
    {
        try {
            sol::function f = (*_state)[function];
            if (!f.valid()) {
                std::cerr << "Not a valid function name " << function << std::endl;
            }
            std::vector<sol::object> modifiedArgs(args.begin(), args.end());
            for (size_t i = 0; i < modifiedArgs.size(); ++i) {
                handleTypeTransformation(modifiedArgs, i);
            }
            auto res = f(sol::as_args(modifiedArgs));
            sol::object obj = res;

            if (obj.is<sol::table>() && !obj.is<sol::userdata>()) {
                return deserializeTable(obj.as<sol::table>());
            }
            return res;
            //return objectToVariant(res);
        } catch (sol::error& error) {
            std::cerr << error.what() << std::endl;
        }
        return sol::nil_t();
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

