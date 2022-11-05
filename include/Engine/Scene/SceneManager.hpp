#pragma once
#include "Scene.hpp"
#include <queue>

class SceneManager {
    public:
        SceneManager() = default;
        ~SceneManager() = default;

        void addScene(Scene const& scene);
        void popScene();
        Scene const& getScene();

        void create();
        void update();
        void destroy();

    private:
        std::queue<Scene> _sceneQueue;
};
