#pragma once
#include <queue>
#include "Scene.hpp"

class SceneManager {
    public:
        SceneManager() = default;
        ~SceneManager() = default;

        void addScene(Scene const& scene);
        void popScene();
        Scene& getScene();

        void create();
        void update();
        void destroy();

    private:
        std::queue<Scene> _sceneQueue;
};
