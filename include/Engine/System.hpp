#pragma once
#include <memory>
#include <thread>
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/EntitySignature.hpp"
#include "Components/Sprite.hpp"
#include "Components/Gravity.hpp"
#include "QuadTree.hpp"
#include "Time.hpp"
#include "Layer.hpp"
#include "Entity.hpp"

using SharedEntity = std::shared_ptr<Entity>;

class System {
public:
    System() = default;
    ~System() = default;

    static void addEntity(Entity *entity)
    {
        entity->addComponent<Id>(_id++);
        entity->addComponent<Layer>(0);
        if (_registry_size >= 2) {
            auto it = _registry.begin();

            for (auto e : _registry) {
                auto v = e->getComponent<Layer>()->value();
                auto v2 = entity->getComponent<Layer>()->value();

                if (v > v2) {
                    _registry.insert(it, entity);
                    _registry_size++;
                    return;
                }
                it++;
            }
        }
        _registry.push_back(entity);
        _registry_size++;
    }

    static Entity *getEntity(std::size_t const& id)
    {
        return _registry[id];
    }

    static Entity *getEntity(std::string const& signature)
    {
        for (auto& e : _registry) {
            EntitySignature *esignature = e->getComponent<EntitySignature>();
            std::string ssignature = "";

            if (!esignature)
                continue;
            ssignature = esignature->signature();
            if (ssignature.find(signature) != std::string::npos)
                return e;
        }
        return nullptr;
    }

    static int RemoveEntity(Entity *e)
    {
        for (auto it = _registry.cbegin(); it != _registry.cend(); ) {
            Entity *en = *it;

            if (e == en) {
                _registry.erase(it);
                _registry_size--;
                return 0;
            }
            it++;
        }
        return -1;
    }

    static void refresh_quad()
    {
        // _quad->clear();
        for (auto &e : _registry) {
            auto box = e->getComponent<Transform2D>();
            auto transform = e->getComponent<Transform2D>();

            if (box && transform)
                _quad->insert(e);
        }
        std::cout << _registry.size() << std::endl;
    }

    bool isInView(Entity *e);

    // System that apply force such has velocity and all

    void init();

    void merge();

    void velocity_system(Entity *e);

    void quad_collision_system();

    void BoxSystem(Entity *e);

    void collider_system();

    void collider_system_check_entity(Entity *entity, BoxCollider *collider, Velocity<float> *velocity);

    void draw_system();

    void gravity_system(Entity *e);

    void update_custom_component(Entity *e);

    void camera_system(Entity *e);

    void systems();

private:
    static void sort();

private:
    static inline Quadtree *_quad = new Quadtree(sf::FloatRect(0, 0, 300, 300));
    static inline std::size_t _id = 0;
    static inline std::vector<Entity *> _registry;
    static inline int _registry_size = 0;
};