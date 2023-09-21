#pragma once
#include <memory>
#include <thread>
#include <functional>
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/EntitySignature.hpp"
#include "Components/Sprite.hpp"
#include "Components/Gravity.hpp"
#include "Collision/QuadTree.hpp"
#include "Collision/Layer/TileMap.hpp"
#include "Time.hpp"
#include "Layer.hpp"
#include "Entity.hpp"

using SharedEntity = std::shared_ptr<Entity>;

class System {
public:
    System() = default;
    ~System() = default;

    static void addEntity(Entity *entity);

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

    static void addTileMap(TileMap *layer)
    {
        _layers.push_back(layer);
    }

    static void addInDrawQueue(Sprite *sprite)
    {
        _drawQueue.push_back(sprite);
    }

    bool isInView(Entity *e);

    // System that apply force such has velocity and all

    void init();

    void merge();

    void velocity_system(Entity *e);

    void quad_collision_system();

    void BoxSystem(Entity *e);

    void collider_system(Entity *e);

    void gravity_system(Entity *e);

    void update_custom_component(Entity *e);

    void camera_system(Entity *e);

    void light_system(Entity *e);

    void sprite_system(Entity *e, std::vector<IComponent *> componentCache);

    void systems();

public:
    static inline int lightSources = 0;

private:
    static void sort();

private:
    std::vector<Entity *> _inView;
    static inline std::vector<Sprite *> _drawQueue;
    static inline Quadtree *_quad = new Quadtree(sf::FloatRect(0, 0, 300, 300));
    static inline std::size_t _id = 0;
    static inline std::vector<Entity *> _registry;
    static inline int _registry_size = 0;
    static inline std::vector<TileMap *> _layers;
};