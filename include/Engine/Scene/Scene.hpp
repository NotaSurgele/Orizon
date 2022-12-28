#pragma once

#include "Engine/Scene/IScene.hpp"
#include "Engine/System.hpp"
#include "Engine/Core.hpp"

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

    int loadSceneFromFile(const std::string& file)
    {
        return 0;
    }

    //template <typename T>
    //static Entity* getEntity(Signature signature)
    //{
    //    return System::getEntity<T *>(signature);
    //}

};
