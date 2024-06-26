#include "Engine/Scene/SceneManager.hpp"

void SceneManager::addScene(Scene *scene)
{
    _sceneQueue.push(scene);
}

void SceneManager::popScene()
{
    return _sceneQueue.pop();
}

Scene *SceneManager::getScene()
{
    return _sceneQueue.front();
}

void SceneManager::create()
{
    _sceneQueue.front()->start();
}

void SceneManager::update()
{
    _sceneQueue.front()->update();
}

void SceneManager::destroy()
{
    _sceneQueue.front()->destroy();
    //popScene();
}
