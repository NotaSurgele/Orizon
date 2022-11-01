#pragma once
#include <memory>
#include "Entity.hpp"
#include "Core.hpp"

using SharedEntity = std::shared_ptr<Entity *>;

class System {
public:
    System() = default;
    ~System() = default;

    template <typename T>
    static void addEntity(T *entity)
    {
        _registry[SIGNATURE(T)] = std::make_shared<T *>(entity);
    }

    template <typename T, class... Args>
    static void addEntity(Args... args)
    {
        _registry[SIGNATURE(T)] = std::make_shared<T *>(args ...);
    }

    template <typename T>
    static std::shared_ptr<T *> getEntity(Signature signature)
    {
        return _registry[signature];
    }


    template <typename T>
    static std::shared_ptr<T *> getEntityByType()
    {
        return _registry[SIGNATURE(T)];
    }

private:
    static inline std::unordered_map<Signature, SharedEntity> _registry;
};
