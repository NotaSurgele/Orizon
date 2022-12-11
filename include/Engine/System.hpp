#pragma once
#include <memory>
#include "Components/Velocity.hpp"
#include "Time.hpp"
#include "Components/Sprite.hpp"
#include "Entity.hpp"

using SharedEntity = std::shared_ptr<Entity *>;

class System {
public:
    System() = default;
    ~System() = default;

    static void addEntity(Entity *entity)
    {
        entity->addComponent<Id>(_id);
        _registry[_id++] = std::make_shared<Entity *>(entity);
    }

    static std::shared_ptr<Entity*> getEntity(std::size_t const& id)
    {
        return _registry[id];
    }

    // System that apply force such has velocity and all
    void velocity_system();

    void collider_system();

    void draw_system();

    void systems();

private:
    static inline std::size_t _id = 0;
    static inline std::unordered_map<std::size_t, SharedEntity> _registry;
};
