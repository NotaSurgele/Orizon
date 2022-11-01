#pragma once

#include "Engine/Scene/IScene.hpp"
#include "Engine/System.hpp"

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
        System::addEntity(entity);
    }

    template <typename T, class... Args>
    static void addEntity(Args... args)
    {
        System::addEntity(args ...);
    }

    template <typename T>
    static T* getEntity(Signature signature)
    {
        return System::getEntity<T *>(signature);
    }

};
