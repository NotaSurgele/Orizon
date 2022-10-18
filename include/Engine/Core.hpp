#pragma once
#include "RessourcesManager.hpp"
#include "Engine/Time.hpp"

class Core {
    public:
        static inline Time time = Time();

        Core();
        ~Core() = default;

        static RessourcesManager& RManager()
        {
            return _r_manager;
        }

        // void start();
        // void update();
        // void render();

    private:
        static inline RessourcesManager _r_manager;
};
