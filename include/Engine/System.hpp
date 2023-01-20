#pragma once
#include <memory>
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/EntitySignature.hpp"
#include "Components/Sprite.hpp"
#include "Components/Gravity.hpp"
#include "Time.hpp"
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

    static std::shared_ptr<Entity *> getEntity(std::size_t const& id)
    {
        return _registry[id];
    }

    static Entity *getEntity(std::string const& signature)
    {
        for (auto& e : _registry) {
            EntitySignature *esignature = (*e.second.get())->getComponent<EntitySignature>();
            std::string ssignature = "";

            if (!esignature)
                continue;
            ssignature = esignature->signature();
            if (ssignature.find(signature) != std::string::npos)
                return (*e.second.get());
        }
        return nullptr;
    }

    static int RemoveEntity(Entity *e)
    {
        for (auto it = _registry.cbegin(); it != _registry.cend(); ) {
            Entity *en = *it->second;

            if (e == en) {
                _registry.erase(it);
                return 0;
            }
            it++;
        }
        return -1;
    }

    // System that apply force such has velocity and all
    void velocity_system();

    void collider_system();

    void collider_system_check_entity(Entity *entity, BoxCollider *collider, Velocity<float> *velocity);

    void draw_system();

    void gravity_system();

    void update_custom_component();

    void camera_system();

    void systems();

private:
    static inline std::size_t _id = 0;
    static inline std::unordered_map<std::size_t, SharedEntity> _registry;
};
