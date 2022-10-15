#pragma once
#include "RessourcesManager.hpp"

class Core {
    public:
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
