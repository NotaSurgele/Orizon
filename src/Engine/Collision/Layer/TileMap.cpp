#include "Collision/Layer/TileMap.hpp"
#include "Engine/Components/BoxCollider.hpp"
#include "Engine/System.hpp"
#include "Engine/Core.hpp"
#include <cstddef>

TileMap::TileMap(   const float& x,
                    const float& y,
                    const int& width,
                    const int& height,
                    const int& tileWidth,
                    const int& tileHeight)
                    : w(width),
                        h(height),
                        tileWidth(tileWidth),
                        tileHeight(tileHeight),
                        _x(x),
                        _y(y)
{
    for (std::size_t i = 0; i < height; i++) {
        std::vector<Entity *> row;
        for (std::size_t e = 0; e < width; e++) {
            row.push_back(nullptr);
        }
        _layer.push_back(row);
    }
    System::addTileMap(this);
}

TileMap::~TileMap()
{
    for (std::size_t row = 0; row < w; row++) {
        _layer[row].clear();
    }
    _layer.clear();
}

sf::Vector2f TileMap::emptySpot(int range)
{
    std::size_t counter = 0;

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            if (_layer[x][y] == nullptr)
                return sf::Vector2f(((x - (range / 2)) * tileWidth) + 50, (y - (range / 2)) * tileHeight);
        }
    }
    return sf::Vector2f(0, 0);
}

std::vector<Entity *> TileMap::getAllEntities()
{
    return _entities;
}

bool TileMap::emplaceEntity(Entity *e)
{
    auto position = e->getComponent<Transform2D>()->position;
    int x = position.x / tileWidth;
    int y = position.y / tileHeight;

    if (x < 0 || x > w || y < 0 || y > h)
        return false;
    _layer[x][y] = e;
    _entities.push_back(e);
    return true;
}

bool TileMap::removeEntity(const int& x, const int& y)
{
    auto e = _layer[x][y];
    std::size_t size = _entities.size();

    for (std::size_t i = 0; i < size; i++) {
        if (e == _entities[i]) {
            _entities.erase(_entities.begin() + i);
            break;
        }
    }
    _layer[x][y] = nullptr;
    return true;
}

bool TileMap::contain(Entity *e)
{
    auto position = e->getComponent<Transform2D>()->position;
    sf::FloatRect bounds = { _x, _y, (float)w, (float)h };
    sf::RectangleShape rect = sf::RectangleShape();
    rect.setSize(sf::Vector2f(w, h));
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(1.0f);
    DRAW(rect);
    return bounds.contains(position.x, position.y);
}

bool TileMap::contain(const float& x, const float& y)
{
    sf::FloatRect bounds = { this->_x, this->_y, (float)this->w, (float)this->h };
    return bounds.contains(x, y);
}

void TileMap::display()
{
    for (std::size_t x = 0; x < w; x++) {
        for (std::size_t y = 0; y < h; y++) {
            _layer[x][y] != nullptr ? std::cout << "0" : std::cout << "1";
        }
        std::cout << std::endl;
    }
}