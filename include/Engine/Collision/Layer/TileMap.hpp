#pragma once

#include "Engine/Entity.hpp"
#include <Transform2D.hpp>
#include <tuple>
#include <iostream>

class TileMap {
public:
    TileMap(const float& x, const float& y, const int& width, const int& height, const int& tileWidth, const int& tileHeight);
    ~TileMap();

    bool emplaceEntity(Entity *e);
    bool contain(Entity *e);
    bool contain(const float& x, const float& y);
    bool isInside(Entity *e);
    bool removeEntity(const int& x, const int& y);
    bool removeEntity(Entity *e);
    bool isRender();
    void render();
    void hide();

    sf::Vector2f emptySpot(int range);
    std::vector<Entity *> getAllEntities();

    std::vector<Entity *> checkAround(Entity *e, int range);

    template<typename T>
    std::vector<std::vector<T>> getLayerInfo()
    {
        std::vector<T> arr;

        for (std::size_t i = 0; i < _layer.size(); i++) {
            std::vector<T> lines;

            for (std::size_t j = 0; j < _layer[i].size(); j++) {
                lines.push_back(_layer[i][j] != nullptr);
            }
            arr.push_back(lines);
            lines.clear();
        }
        return arr;
    }

    std::vector<std::vector<Entity *>> getLayerInfo()
    {
        return _layer;
    }

    template <typename... Others>
    std::vector<Entity *> checkEdges(Entity *e, int range)
    {
        std::vector<Entity *> arr;
        auto transform = e->getComponent<Transform2D>();

        if (!transform)
            return arr;
        auto position = transform->position;
        auto size = transform->size;
        int baseX = position.x / tileWidth;
        int baseY = position.y / tileHeight;
        int x = baseX - range;
        int y = baseY - range;

        for (std::size_t x2 = x; x2 <= (baseX + range); x2++) {
            for (std::size_t y2 = y; y2 <= (baseY + range); y2++) {
                auto block = _layer[x2][y2];
                auto entity = _layer[x2][y2];

                int directionX = 0;
                int directionY = 0;

                if (block == nullptr) {
                    y2++;
                    continue;
                }

                // Check around the block if there is an avalaible place depending on the entity position
                if (!entity->hasComponents<Others...>()) {
                    continue;
                }
                auto blockPosition = block->getComponent<Transform2D>()->position;
                if (position.x < blockPosition.x)
                    directionX = -1;
                else
                    directionX = 1;
                if (position.y < blockPosition.y)
                    directionY = -1;
                else
                    directionY = 1;
                if (_layer[x2 + directionX][y2 + directionY] == nullptr)
                    arr.push_back(entity);
            }
        }
        return arr;
    }

    template <typename... Others>
    std::vector<Entity *> checkAround(Entity *e, int range)
    {
        std::vector<Entity*> arr;
        auto position = e->getComponent<Transform2D>()->position;

        int x = position.x / tileWidth;
        int y = position.y / tileHeight;

        int minX = std::max(0, x - range);
        int maxX = std::min(static_cast<int>(_layer.size()) - 1, x + range);
        int minY = std::max(0, y - range);
        int maxY = std::min(static_cast<int>(_layer[0].size()) - 1, y + range);

        for (int x2 = minX; x2 <= maxX; x2++) {
            for (int y2 = minY; y2 <= maxY; y2++) {
                auto entity = _layer[x2][y2];

                if (entity == e)
                    continue;
                if (entity != nullptr) {
                    if (!entity->hasComponents<Others ...>())
                        continue;
                    arr.push_back(entity);
                }
            }
        }
        return arr;
    }

    void destroy()
    {
        for (auto cols : _layer) {
            for (auto cell : cols) {
                cell->destroy();
            }
            cols.clear();
        }
        _layer.clear();
        _entities.clear();
    }

    void outputValues();

public:
    int w = 0;
    int h = 0;
    int tileWidth = 0;
    int tileHeight = 0;
    float _x = 0;
    float _y = 0;

private:
    bool _isRender = false;
    std::vector<Entity *> _entities;
    std::vector<std::vector<Entity *>> _layer;
};