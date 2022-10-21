#pragma once
#include "Engine/Scene/IScene.hpp"
#include <queue>

class SceneManager {
    public:
        SceneManager() = default;
        ~SceneManager() = default;

        void addScene(IScene const& scene);
        void popScene();
        IScene const& getScene();

        void create();
        void update();
        void destroy();

    private:
        std::queue<IScene> _sceneQueue;
};