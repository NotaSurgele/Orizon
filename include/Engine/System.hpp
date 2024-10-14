#pragma once
#include <memory>
#include <functional>
#include <thread>
#include <semaphore>
#include <list>

// internal
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Tag.hpp"
#include "Components/Sprite.hpp"
#include "Components/Gravity.hpp"
#include "Components/Particles.hpp"
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
    static void pushEntity(Entity *entity);
    static void forceUpdate(Entity *e);
    static void forceDestroy();

    static Entity *getEntity(std::size_t const& id)
    {
        for (auto& e : _registry) {
            auto id = e->getComponent<Id>();
            if (id == id) return e;
        }
        return nullptr;
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

    static std::list<Entity *> getEntitiesFromTag(const std::string& tagName)
    {
        std::list<Entity *> results;
        for (auto &e : _registry) {
            Tag *name = e->getComponent<Tag>();

            if (name->value().find(tagName) != std::string::npos)
                results.push_back(e);
        }
        return results;
    }

    static std::list<Entity *> setRegistry(const std::list<Entity *>& newRegistry)
    {
        std::list<Entity *> old = _registry;
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

    static std::list<Entity *> getDynamicEntities()
    {
        return _dynamic_collider;
    }

    static int RemoveEntity(Entity *e)
    {
        _to_destroy.push_back(e);
        return 0;
    }

    static void DestroyTileMap(TileMap *map)
    {
        _destroy_tilemap.push_back(map);
    }

    static void __registerScriptedEntity(Entity *e)
    {
        if (!e) return;
        _scripted_entity.push_back(e);
    }

//#ifdef SYSTEM_CALLER

    static void __registerDynamicCollider(Entity *other)
    {
        auto collider = other->getComponent<BoxCollider>();

        if (!collider->___isSet) {
            auto find = std::find(_dynamic_collider.begin(), _dynamic_collider.end(), other);
            if (find != _dynamic_collider.end()) {
                return;
            }
            _dynamic_collider.push_back(other);
        }
    }

    static void __removeDynamicCollider(Entity *e)
    {
        _dynamic_collider.erase(std::remove(_dynamic_collider.begin(),
                                    _dynamic_collider.end(), e), _dynamic_collider.end());
    }

    static void __addCanvas(Entity *e)
    {
        _canvas.push_back(e);
    }

    static void __removeCanvas(Entity *e)
    {
        _canvas.erase(std::remove(_canvas.begin(), _canvas.end(), e), _canvas.end());
    }

    static void __addLightSource(Entity *e)
    {
        _lightSource.push_back(e);
    }

    static void __removeLightSource(Entity *e)
    {
        _lightSource.erase(std::remove(_lightSource.begin(), _lightSource.end(), e), _lightSource.end());
    }

    static void ___insert_entity_at_location(Entity *e)
    {
        auto toInsertLayerValue = e->getComponent<Layer>()->value();

        // if the layer value already exist
        if (_orders_values.contains(toInsertLayerValue)) {
            //std::cout << "Contain value " << value << "at position " << _orders_values[value] << std::endl;

            for (auto& v : _orders_values) {
                auto& layerValue = v.first;
                auto& registryRange = v.second;

                if (layerValue >= toInsertLayerValue) {
                    auto& end = registryRange.second;

                    if (layerValue > toInsertLayerValue) {
                        auto& start = registryRange.first;

                        start++;
                    }
                    end++;
                }
            }
            return;
        }

        std::size_t tmpLayerValue = 0;
        std::pair oldValue = {-1, -1};

        for (auto& values : _orders_values) {
            auto layerValue = values.first;

            if (toInsertLayerValue < layerValue) {
                // Range 1 : 0 - 180 | Range 3: 180 - 300 | actual layer value = 2
                int start = 0;
                int end = 0;

                if (_orders_values.contains(tmpLayerValue)) {
                    auto& before = _orders_values[tmpLayerValue];

                    start = before.second + 1;
                }

                // start = 181
                end = start + 1;
                // end = 182

                _orders_values.insert(std::pair(toInsertLayerValue,
                    std::pair(start, end)));

                // Update greater values
                for (auto& v : _orders_values) {
                    if (v.first > toInsertLayerValue) {
                        auto& range = v.second;

                        range.first += 1;
                        range.second += 1;
                    }
                }
                return;
            }
            oldValue = _orders_values[layerValue];
            tmpLayerValue = layerValue;
        }
        // if value is greater than everything in the array then insert at the end
        _orders_values.insert(std::pair<std::size_t,
            std::pair<int, int>>(toInsertLayerValue, std::pair(oldValue.second + 1, oldValue.second + 2)));
    }

//#endif // SYSTEM_CALLER


    static void addTileMap(TileMap *layer)
    {
        _layers.push_back(layer);
    }

    static sf::Vector2f getLocalMousePosition();
    static sf::Vector2f getGlobalMousePosition();

    static sf::Vector2f localToGlobalCoordinate(const sf::Vector2f& local);
    static sf::Vector2f globalToLocalCoordinate(const sf::Vector2f& global);

    std::list<Entity *> getRegistry() { return _forceUpdate; }

    std::vector<TileMap *> getTileMaps() {  return _layers; }

    void systems();

public:
    static inline int lightSources = 0;

private:
    static void sort();

    bool isInView(Entity *e);
    bool isInView(TileMap *map);

    void merge();

    void velocitySystem(Entity *e);

    void BoxSystem(Entity *e);

    void colliderSystem(Entity *e);

    void collisionResolution(BoxCollider *box, BoxCollider *collider);

    void gravitySystem(Entity *e);

    void updateCustomComponent(Entity *e);

    void cameraSystem(Entity *e);

    void lightSystem(Entity *e);

    void particleEmitterSystem(Entity *e);

    void spriteSystem(Entity *e, std::vector<IComponent *> componentCache);

    void canvasSystem(Entity *e);

    void scriptSystem(Entity *e);

    //Component

    // Light
    void handleSpriteLightning(Sprite *sprite, Light *light);
    bool lightLayerRaycast(Light *light, Entity *e);
    void setSpriteShadow(Entity *e);


    void clearComponentCache(const std::vector<IComponent *> &componentCache);

    // Collider
    void handleLayerCollision(BoxCollider *box, int range, Entity *e);
    void handleDynamicEntityCollision(Entity *e, BoxCollider *box);

    //Collision
    bool resolutionCalculation(BoxCollider *box, BoxCollider *collider, Entity *entity);

    // Velocity
    void handleVelocityCollidingSides(BoxCollider *box, Transform2D *transform, Velocity *velocity);

    // Destroy
    void destroyEntity();

private:
    static inline HashGrid *_hashGrid = new HashGrid();
    static inline std::size_t _id = 0;
    static inline std::list<Entity *> _registry;
    static inline int _registry_size = 0;
    static inline std::vector<TileMap *> _layers;
    static inline std::list<Entity *> _dynamic_collider;
    static inline std::list<Entity *> _scripted_entity;
    static inline std::list<Entity *> _canvas;
    static inline std::map<std::size_t, std::pair<int, int>> _orders_values;
    static inline std::list<Entity *> _to_destroy;
    static inline std::vector<TileMap *> _destroy_tilemap;
    static inline std::list<Entity *> _lightSource;
    static inline std::list<Entity *> _forceUpdate;
};