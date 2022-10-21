#pragma once
#include "RessourcesManager.hpp"
#include "Engine/Time.hpp"

class Core {
    public:

        Core();
        ~Core() = default;

        static RessourcesManager& RessourceManager()
        {
            return _r_manager;
        }

        static float getDeltaTime()
        {
            return _time.getDeltaTime();
        }

        // void start();
        // void update();
        // void render();

        static inline Time _time = Time();
    private:
        static inline RessourcesManager _r_manager;
};

/**
 * @brief macro to push the given ressource in the ressource manager
 * @retval these macro as no return value
 */
#define R_ADD_RESSOURCE(type, name, path) \
        Core::RessourceManager().addRessource<type>(name, path);

/**
 * @brief  macro to retrieve a ressouce from name
 * @retval the ressource asked
 */
#define R_GET_RESSOURCE(type, name) \
        Core::RessourceManager().getRessource<type>(name)
