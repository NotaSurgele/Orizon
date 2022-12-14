#include "Engine/Scene/SceneManager.hpp"

void SceneManager::addScene(Scene const& scene)
{
    _sceneQueue.push(scene);
}

void SceneManager::popScene()
{
    return _sceneQueue.pop();
}

Scene const& SceneManager::getScene()
{
    return _sceneQueue.front();
}

void SceneManager::create()
{
    _sceneQueue.front().create();
}

void SceneManager::update()
{
    _sceneQueue.front().update();
}

void SceneManager::destroy()
{
    _sceneQueue.front().destroy();
}
