#include "TiledMap.hpp"
#include "Utils.hpp"
#include "Core.hpp"

bool TiledMap::load(const std::string &filePath)
{
    std::string fileContent = Utils::readFile(filePath);

    if (fileContent.size() <= 0)
        return false;
    _mapContent = Utils::fileToJson(fileContent);
    _loadTileSet();
    _loadTileMap();
    return true;
}

void TiledMap::destroy()
{
    std::destroy(_mapContent.begin(), _mapContent.end());
    for (auto tilemap : _tileMaps) {
        tilemap->destroy();
    }
}

bool TiledMap::_loadTileMap()
{
    auto layers = _mapContent["layers"];

    for (auto layer : layers) {
        int height = layer["height"];
        int width = layer["width"];
        int x = layer["x"];
        int y = layer["y"];
        auto cells = layer["data"];

        TileMap *tilemap = new TileMap(x, y, width *
        _tileWidth, height * _tileHeight, _tileWidth, _tileHeight);
        int index = 0;
        float posX = 0;
        float posY = 0;
        for (auto cell : cells) {
            int cellId = cell;

            if (index >= 20) {
                index = 0;
                posX = 0;
                posY += _tileHeight;
            }
            sf::Texture texture = R_GET_RESSOURCE(sf::Texture, std::to_string(cellId));
            Entity *e = new Entity();

            e->addComponent<Transform2D>()->position = sf::Vector2f(posX, posY);
            e->addComponent<Sprite>(texture);
            tilemap->emplaceEntity(e);
            _tileMaps.push_back(tilemap);
            posX += _tileWidth;
            index++;
        }
    }
    return true;
}

void TiledMap::_loadTileSet()
{
    auto tilesets = _mapContent["tilesets"];
    for (auto& tileset : tilesets) {
        std::string source = tileset["source"];

        // Handle tileset parsing
        std::string tilesetInfoStr = Utils::readFile("../assets/Maps/" + source);
        nlohmann::json tilesetInfo = Utils::fileToJson(tilesetInfoStr);

        int imgHeight = tilesetInfo["imageheight"];
        int imgWidth = tilesetInfo["imagewidth"];
        std::string img = tilesetInfo["image"];
        int tileWidth = tilesetInfo["tilewidth"];
        int tileHeight = tilesetInfo["tileheight"];
        int id = 1;

        _tileWidth = tileWidth;
        _tileHeight = tileHeight;
        for (int y = 0; y < imgHeight; y += tileHeight) {
            for (int x = 0; x < imgWidth; x += tileWidth) {
                R_ADD_TILE(std::to_string(id), img, x, y, tileWidth, tileHeight);
                id++;
            }
        }
    }
}
