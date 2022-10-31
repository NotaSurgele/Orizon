#pragma once
#include "RessourcesManager.hpp"
#include "Engine/Time.hpp"
#include "Engine/RenderWindow.hpp"
#include "Engine/Input.hpp"
#include "Engine/Components/Drawable.hpp"
#include "Engine/Entity.hpp"
#include <SFML/System.hpp>
#include <string>
#include <unordered_map>

class ICore {
public:
    virtual void start() = 0;
    virtual void render() = 0;
    virtual void destroy() = 0;
};

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

        void loadInputFromFile(std::string const& path);

        template <typename T>
        static void addEntity(T *entity)
        {
            _registry[SIGNATURE(T)] = entity;
        }

        template <typename T, class... Args>
        static void addEntity(Args... args)
        {
            T* newEntity = new T(args ...);
            _registry[SIGNATURE(T)] = newEntity;
        }

        template <typename T>
        static T* getEntity(Signature const& signature)
        {
            return _registry[signature];
        }

        //Window related function
        bool isOpen();
        void CoreClear(sf::Color color);
        void CoreDraw(Drawable *component);
        void CoreClose();

        void run();
        virtual void start() = 0;
        virtual void render() = 0;
        virtual void destroy() = 0;

    private:
        bool CoreEvent(sf::Event& event);
        void CoreDisplay();

    private:
        static inline Time _time;
        static inline RessourcesManager _r_manager;
        static inline std::unordered_map<Signature, Entity *> _registry;

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
