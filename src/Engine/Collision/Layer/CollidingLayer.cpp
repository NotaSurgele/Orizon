#include "Collision/Layer/CollidingLayer.hpp"
#include "Engine/Components/BoxCollider.hpp"
#include "Engine/System.hpp"
#include "Engine/Core.hpp"
#include <cstddef>

CollidingLayer::CollidingLayer( const float& x,
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
    System::addCollidingLayer(this);
}

CollidingLayer::~CollidingLayer()
{
    for (std::size_t row = 0; row < w; row++) {
        _layer[row].clear();
    }
    _layer.clear();
}

sf::Vector2f CollidingLayer::emptySpot(int range)
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

std::vector<Entity *> CollidingLayer::getAllEntities()
{
    return _entities;
}

bool CollidingLayer::emplaceEntity(Entity *e)
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

bool CollidingLayer::removeEntity(const int& x, const int& y)
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

bool CollidingLayer::contain(Entity *e)
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

bool CollidingLayer::contain(const float& x, const float& y)
{
    sf::FloatRect bounds = { this->_x, this->_y, (float)this->w, (float)this->h };
    return bounds.contains(x, y);
}

std::vector<Entity *> CollidingLayer::checkAround(Entity *e, int range)
{
    std::vector<Entity *> arr;
    auto position = e->getComponent<BoxCollider>()->getPosition();

    int x = position.x / tileWidth;
    int y = position.y / tileHeight;

    for (std::size_t x2 = x - range; x2 <= (x + range); x2++) {
        for (std::size_t y2 = y - range; y2 <= (y + range); y2++) {
            if (_layer[x2][y2] != nullptr)
                arr.push_back(_layer[x2][y2]);
        }
    }
    return arr;
}

std::vector<Entity *> CollidingLayer::checkEdges(Entity *e, int range)
{
    std::vector<Entity *> arr;
    auto collider = e->getComponent<BoxCollider>();

    if (!collider)
        return arr;
    auto position = collider->getPosition();
    int baseX = position.x / tileWidth;
    int baseY = position.y / tileHeight;
    int x = baseX - range;
    int y = baseY - range;

    for (std::size_t x2 = x; x2 <= (baseX + range); x2++) {
        for (std::size_t y2 = y; y2 <= (baseY + range); y2++) {
            auto block = _layer[x2][y2];
            int directionX = 0;
            int directionY = 0;

            if (block == nullptr) {
                y2++;
                continue;
            }

            auto blockPosition = block->getComponent<Transform2D>()->position;
            // Check around the block if there is an avalaible place depending on the entity position
            if (position.x < blockPosition.x)
                directionX = -1;
            else if (position.x > blockPosition.x)
                directionX = 1;
            if (position.y < blockPosition.y)
                directionY = -1;
            else if (position.y > blockPosition.y)
                directionY = 1;
            if (_layer[x2 + directionX][y2 + directionY] == nullptr) {
                arr.push_back(_layer[x2][y2]);
            }
        }
    }
    return arr;
}

void CollidingLayer::display()
{
    for (std::size_t x = 0; x < w; x++) {
        for (std::size_t y = 0; y < h; y++) {
            _layer[x][y] != nullptr ? std::cout << "0" : std::cout << "1";
        }
        std::cout << std::endl;
    }
}
