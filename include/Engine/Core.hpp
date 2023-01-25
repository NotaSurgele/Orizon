#pragma once
#include "RessourcesManager.hpp"
#include "Engine/Time.hpp"
#include "Engine/RenderWindow.hpp"
#include "Engine/Input.hpp"
#include "Engine/Components/Drawable.hpp"
#include "Engine/Entity.hpp"
#include "Engine/System.hpp"
#include "external/json.hpp"
#include <SFML/System.hpp>
#include <string>
#include <unordered_map>

class ICore {
public:
    virtual void start() = 0;
    virtual void render() = 0;
    virtual void destroy() = 0;
};

class View;

using Signature = const char *;

class Core : public ICore {
    public:
        Core() = delete;
        Core(std::string const& windowName, std::size_t width=800, std::size_t height=600);
        ~Core() = default;

        static RessourcesManager& RessourceManager()
        {
            return _r_manager;
        }

        void setView(View *view);
        void loadInputFromFile(std::string const& path);

        //Window related function
        bool isOpen();
        RenderWindow& getWindow();
        void CoreClear(sf::Color color);
        void CoreDraw(Drawable *component);
        void CoreClose();


        void run();
        virtual void start() = 0;
        virtual void render() = 0;
        virtual void destroy() = 0;

public:
    static inline Core *instance;

    private:
        bool CoreEvent(sf::Event& event);
        void CoreDisplay();

    private:
        static inline Time _time;
        static inline RessourcesManager _r_manager;

        //Utils
        RenderWindow _window;
        Input _input;
        System _system_handler;
};

/**
 * @brief macro to push the given ressource in the ressource manager
 * @retval these macro as no return value
 */
#define R_ADD_RESSOURCE(type, name, path) \
        Core::RessourceManager().addRessource<type>(name, path)

#define R_ADD_TILE(name, path, x, y, w, h)\
        Core::RessourceManager().loadTileFromSpriteSheet(name, path, x, y, w, h)
/**
 * @brief  macro to retrieve a ressouce from name
 * @retval the ressource asked
 */
#define R_GET_RESSOURCE(type, name) \
        Core::RessourceManager().getRessource<type>(name)

#define SET_VIEW(view) \
        Core::instance->setView(view)

#define Window \
        Core::instance->getWindow()

#define CLOSE()\
        Core::instance->CoreClose();

#define DRAW(to_draw)\
        Core::instance->CoreDraw(to_draw)

#define CORE\
        Core::instance
