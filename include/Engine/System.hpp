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
        _registry[_id] = std::make_shared<T *>(entity);
    }

    template <typename T, class... Args>
    static void addEntity(Args... args)
    {
        _registry[_id++] = std::make_shared<T *>(args ...);
    }

    template <typename T>
    static std::shared_ptr<T *> getEntity(std::size_t const& id)
    {
        return _registry[id];
    }


private:
    static inline std::size_t _id = 0;
    static inline std::unordered_map<std::size_t, SharedEntity> _registry;
};
