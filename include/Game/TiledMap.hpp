#pragma once

#include <string>
#include "json.hpp"
#include "TileMap.hpp"

class TiledMap {
public:
    TiledMap() = default;
    ~TiledMap() = default;

    bool load(const std::string& filePath);
    void destroy(void);

private:
    bool _loadTileMap();
    void _loadTileSet(void);

private:
    nlohmann::json _mapContent;
    std::vector<TileMap *> _tileMaps;
    int _tileWidth;
    int _tileHeight;
};