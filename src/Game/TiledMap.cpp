#include "TiledMap.hpp"
#include "Utils.hpp"
#include "Core.hpp"

bool TiledMap::load(const std::string &filePath)
{
    for (auto layer : _tileMaps) {
        layer->destroy();
    }
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

    int draw = 0;
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
        std::cout << "DRAW " << draw << std::endl;
        for (auto cell : cells) {
            int cellId = cell;
            if (index >= height) {
                index = 0;
                posY = 0;
                posX += _tileHeight;
            }
            if (cellId > 0) {
                sf::Texture texture = R_GET_RESSOURCE(sf::Texture, std::to_string(cellId));
                Entity *e = new Entity();

                e->getComponent<Layer>()->set(draw);
                e->addComponent<Transform2D>()->position = sf::Vector2f(posY, posX);
                e->addComponent<Sprite>(texture);
                e->addComponent<BoxCollider>(e->getComponent<Transform2D>()->position,
                                             sf::Vector2f(16, 16))->setType(BoxCollider::STATIC);
                e->addComponent<Tag>("map");
                tilemap->emplaceEntity(e);
                //System::pushEntity(e);
            }
            posY += _tileWidth;
            index++;
        }
        _tileMaps.push_back(tilemap);
        draw += 2;
    }
    return true;
}

void TiledMap::_loadTileSet()
{
    auto tilesets = _mapContent["tilesets"];
    for (auto& tileset : tilesets) {
        std::string source = tileset["source"];

        // Handle tileset parsing
        std::string tilesetInfoStr = Utils::readFile("../assets/" + source);
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
