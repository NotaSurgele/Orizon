#pragma once
#include "Entity.hpp"
#include "TileMap.hpp"
#include <vector>

class EngineHud {
public:
    EngineHud(const std::size_t& width, const std::size_t& height) : _width(width), _height(height) {}
    ~EngineHud() = default;

    void entityWindow(const std::vector<Entity *>& _registry, const std::vector<TileMap *>& tileMap);
    void entityInformation();

private:
    void layersEntity(std::size_t& index,  const std::vector<TileMap *>& tileMap);

private:
    Entity *_selected = nullptr;
    std::size_t _width;
    std::size_t _height;
};