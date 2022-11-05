#pragma once

#include "Engine/Scene/IScene.hpp"
#include "Engine/System.hpp"

class Scene : public IScene {
public:
    Scene() = default;
    ~Scene() = default;

    void create() override {};
    void update() override {};
    void destroy() override {};

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
