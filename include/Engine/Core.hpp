#pragma once
#include "RessourcesManager.hpp"
#include "Engine/Time.hpp"
#include "Engine/RenderWindow.hpp"
#include "Engine/Input.hpp"
#include <SFML/System.hpp>
#include <string>
#include "Engine/Components/Drawable.hpp"

class Core {
    public:

        Core(std::string const& windowName, std::size_t width=800, std::size_t height=600);
        ~Core() = default;

        static RessourcesManager& RessourceManager()
        {
            return _r_manager;
        }

        static float getDeltaTime()
        {
            return _time.getDeltaTime();
        }

        static Time& getTime()
        {
            return _time;
        }

        //Window related function
        bool isOpen();
        void CoreClear(sf::Color color);
        bool CoreEvent(sf::Event& event);
        void CoreDraw(Drawable *component);
        void CoreDisplay();
        void CoreClose();
        // void start();
        // void update();
        // void render();

    private:
        static inline Time _time;
        static inline RessourcesManager _r_manager;

        //Utils
        RenderWindow _window;
        Input _input;
};

/**
 * @brief macro to push the given ressource in the ressource manager
 * @retval these macro as no return value
 */
#define R_ADD_RESSOURCE(type, name, path) \
        Core::RessourceManager().addRessource<type>(name, path)

/**
 * @brief  macro to retrieve a ressouce from name
 * @retval the ressource asked
 */
#define R_GET_RESSOURCE(type, name) \
        Core::RessourceManager().getRessource<type>(name)
