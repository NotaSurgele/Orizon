#pragma once
#include "System.hpp"
#include "ResourcesManager.hpp"
#include "Engine/Time.hpp"
#include "Engine/RenderWindow.hpp"
#include "Engine/Input.hpp"
#include "Engine/Components/Drawable.hpp"
#include "Engine/Entity.hpp"
#include "Engine/EngineHud.hpp"
#include "external/json.hpp"
#include "SceneManager.hpp"
#include <SFML/System.hpp>
#include <string>
#include <unordered_map>
#include <thread>

class ICore {
public:
    virtual void start() = 0;
    virtual void render() = 0;
    virtual void destroy() = 0;
};

class View;

class Core : public ICore {
    public:
        Core() = delete;
        Core(std::string const& windowName, std::size_t width=800, std::size_t height=600);
        ~Core() = default;

        static ResourcesManager& resourceManager()
        {
            return _r_manager;
        }

        void setView(View *view);
        void loadInputFromFile(std::string const& path);
        RenderWindow& getWindow();

        //Window related function
        bool isOpen();
        void CoreClear(sf::Color color);
        void CoreDraw(Drawable *component);
        void CoreDraw(Drawable *component, const sf::BlendMode& blendMode);
        void CoreDraw(sf::Drawable const& drawable);
        void CoreDraw(sf::Drawable const& drawable, const sf::BlendMode& blendMode);
        void CoreClose();

        void run();
        virtual void start() override = 0;
        void render() override { _sceneManager.update(); };
        virtual void destroy() override = 0;

    protected:

    private:
        bool CoreEvent(sf::Event& event);
        void CoreDisplay();

        void inputHandler(sf::Event& event);
        void fpsCalculation();

        // ENGINE GUI
        void initGui();
        void updateGUI();
        void destroyGUI();


public:
    static inline Core *instance;
    static inline float fps = 0.0f;

protected:
    static inline SceneManager _sceneManager {};

private:
    static inline Time _time;
    static inline ResourcesManager _r_manager;

    //Utils
    RenderWindow _window;
    Input _input;
    System _system_handler;
    sf::RenderTexture _texture;
    sf::Font font;
    sf::Text fpsText;
    sf::View _hud;
    View *_baseView;
    EngineHud _gui;
    float _fpsTime=1.0f;
    bool _mainViewSelected = true;
    std::thread _guiThread;
};

/**
 * @brief macro to push the given ressource in the ressource manager
 * @retval these macro as no return value
 */
#define R_ADD_RESSOURCE(type, name, path) \
        Core::resourceManager().addRessource<type>(name, path)

#define RESOURCE_MANAGER() \
        Core::resourceManager()
/**
 * @brief macro to push a music inside the ressource manager
*/
#define R_ADD_MUSIC(name, path) \
        Core::resourceManager().loadMusic(name, path)

#define R_ADD_TILE(name, path, x, y, w, h)\
        Core::resourceManager().loadTileFromSpriteSheet(name, path, x, y, w, h)

#define R_ADD_TAG(tag) \
        Core::resourceManager().addTag(tag)

#define R_GET_TAGS() \
        Core::resourceManager().getTags()

#define R_SET_TAG(tag, old) \
        Core::resourceManager().incrementTag(tag, old)
/**
 * @brief load a script and keep it in Resource manager
 */
#define R_ADD_SCRIPT(path)\
        Core::resourceManager().loadScript(path)
/**
 * @brief  macro to retrieve a ressouce from name
 * @retval the ressource asked
 */
#define R_GET_RESSOURCE(type, name) \
        Core::resourceManager().getRessource<type>(name)

#define R_PATH_FROM_NAME(name) \
        Core::resourceManager().pathFromName(name)

#define R_GET_RESSOURCES(type) \
        Core::resourceManager().getRessources<type>()

#define R_GET_MUSIC(name) \
        Core::resourceManager().getMusic(name)

#define R_GET_MUSICS() \
        Core::resourceManager().getMusics()

#define R_GET_SCRIPT(path) \
        Core::resourceManager().getScript(path)

#define R_GET_SCRIPTS() \
        Core::resourceManager().getScripts()

#define SET_VIEW(view) \
        Core::instance->setView(view)

#define WindowInstance \
        Core::instance->getWindow()

#define CLOSE()\
        Core::instance->CoreClose()

#define DRAW(to_draw)\
        Core::instance->CoreDraw(to_draw)

#define DRAW_BLEND(to_draw, blendMode) \
        Core::instance->CoreDraw(to_draw, blendMode)

#define DRAW_QUEUE(to_draw) \
        System::addInDrawQueue(to_draw)

#define CORE\
        Core::instance

#define RANDOM(min, max) \
        std::rand() % (int)(max - min + 1) + min;

#define RANDOMF(min, max) \
        static_cast<float>(std::rand() % (int)(max - min + 1) + min)
