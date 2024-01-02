#pragma once
#include <queue>
#include "Scene.hpp"

class SceneManager {
    public:
        SceneManager() = default;
        ~SceneManager() = default;

        void addScene(Scene* scene);
        void popScene();
        Scene *getScene();

        void create();
        void update();
        void destroy();

        void size()
        {
            std::cout << _sceneQueue.size() << std::endl;
        }

    private:
        std::queue<Scene *> _sceneQueue;
};
