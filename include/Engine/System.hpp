#pragma once
#include <memory>
#include <functional>
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Tag.hpp"
#include "Components/Sprite.hpp"
#include "Components/Gravity.hpp"
#include "Collision/HashGrid.hpp"
#include "Collision/Layer/TileMap.hpp"
#include "Time.hpp"
#include "Layer.hpp"
#include "Light.hpp"

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
            Tag *esignature = e->getComponent<Tag>();
            std::string ssignature = "";

            if (!esignature)
                continue;
            ssignature = esignature->value();
            if (ssignature.find(signature) != std::string::npos)
                return e;
        }
        return nullptr;
    }

    template <typename T>
    static std::vector<T *> getEntitiesFromType()
    {
        std::vector<T *> results;
        for (auto entity : _registry) {
            auto cast = dynamic_cast<T *>(entity);
            if (cast)
                results.push_back(entity);
        }
        return results;
    }

    static std::vector<Entity *> getEntitiesFromTag(const std::string& tagName)
    {
        std::vector<Entity *> results;
        for (auto &e : _registry) {
            Tag *name = e->getComponent<Tag>();

            if (name->value().find(tagName) != std::string::npos)
                results.push_back(e);
        }
        return results;
    }

    static std::vector<Entity *> setRegistry(const std::vector<Entity *>& newRegistry)
    {
        std::vector<Entity *> old = _registry;
        _registry.clear();
        _registry = newRegistry;
        _registry_size = _registry.size();

        for (auto dynamic : _dynamic_collider) {
            auto collider = dynamic->getComponent<BoxCollider>();

            collider->___isSet = false;
        }
        _dynamic_collider.clear();
        return old;
    }

    static std::vector<Entity *> getDynamicEntities()
    {
        return _dynamic_collider;
    }

    static int RemoveEntity(Entity *e)
    {
        _registry.erase(std::remove(
                            _registry.begin(),
                            _registry.end(),
                            e),
                            _registry.end());
        _dynamic_collider.erase(std::remove(
                                    _dynamic_collider.begin(),
                                    _dynamic_collider.end(),
                                    e),
                                    _dynamic_collider.end());
        _hashGrid->remove(e);
        _registry_size--;
        delete e;
        e = nullptr;
        return 0;
    }

    static void __registerDynamicCollider(Entity *other)
    {
        _dynamic_collider.push_back(other);
    }

    static void ___insert_entity_at_location(Entity *e)
    {
        auto layerValue = e->getComponent<Layer>()->value();
        auto exist = _orders_values[layerValue];

        //FIXME entity is not draw in the correct order !!!
        std::thread thread([e]() {
            for (int i = 0; i < _registry_size; i++) {
                if (_registry[i] == e) {
                    _registry.erase(_registry.begin() + i);
                    return;
                }
            }
        });
        thread.join();
        if (exist) {
            auto it = _registry.begin();
            _registry.insert(it + exist, e);
            return;
        }
        int index = 0;
        for (auto pair : _orders_values) {
            auto value = pair.first;
            auto position = pair.second;

            if (layerValue < value) {
                _orders_values[value] = position;
                _registry.insert(_registry.begin() + index, e);
                auto start = _orders_values.begin();

                for (; start != _orders_values.end(); start++) {
                    start->second += 1;
                }
                return;
            }
            index++;
        }
        _orders_values[layerValue] = index;
        auto end = _registry.end();
        _registry.insert(end, e);
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

    void BoxSystem(Entity *e);

    void collider_system(Entity *e);

    void collision_resolution(BoxCollider *box, BoxCollider *collider);

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

    //Component

    // Light
    void handle_sprite_lightning(Sprite *sprite, Light *light);
    bool light_layer_raycast(Light *light, Entity *e);

    void clear_component_cache(const std::vector<IComponent *> &componentCache);

    // Collider
    void handle_layer_collision(BoxCollider *box, int range, Entity *e);
    void handle_dynamic_entity_collision(Entity *e, BoxCollider *box);

    //Collision
    bool resolution_calculation(BoxCollider *box, BoxCollider *collider, Entity *entity);

    // Velocity
    void handle_velocity_colliding_sides(BoxCollider *box, Transform2D *transform, Velocity<float> *velocity);

private:
    std::vector<Entity *> _inView;
    static inline HashGrid *_hashGrid = new HashGrid();
    static inline std::vector<Sprite *> _drawQueue;
    static inline std::size_t _id = 0;
    static inline std::vector<Entity *> _registry;
    static inline int _registry_size = 0;
    static inline std::vector<TileMap *> _layers;
    static inline std::vector<Entity *> _dynamic_collider;
    static inline std::map<std::size_t, int> _orders_values;
};