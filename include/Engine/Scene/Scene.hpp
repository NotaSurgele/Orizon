#pragma once

#include "Engine/Scene/IScene.hpp"
#include "Engine/Core.hpp"

class Scene : public IScene {
public:
    Scene() = default;
    ~Scene() = default;

    virtual void create() = 0;
    virtual void update() = 0;
    virtual void destroy() = 0;

    template <typename T>
    static void addEntity(T *entity)
    {
        Core::addEntity(entity);
    }

    template <typename T, class... Args>
    static void addEntity(Args... args)
    {
        Core::addEntity(args ...);
    }

    template <typename T>
    static T* getEntity(Signature signature)
    {
        return Core::getEntity<T*>(signature);
    }

};
