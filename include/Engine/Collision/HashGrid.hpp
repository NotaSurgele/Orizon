#pragma once
#include <unordered_map>
#include "Entity.hpp"
#include "Components/Sprite.hpp"

class HashGrid {
public:

    HashGrid(float const& cellSize=100, int capacity=100) : _size(0),
                                                              _cellSize(cellSize),
                                                              _capacity(capacity) {
        _grid.reserve(capacity);
    }

    ~HashGrid()
    {
        _grid.clear();
    }

    void insert(Entity* entity) {
        std::vector<sf::Vector2i> cells = calculateCells(entity);

        for (const auto& cell : cells) {
            _grid[cell].push_back(entity);
            _size++;

            // Check if the grid is getting too full and needs to be resized
            if (_size > _capacity) {
                resize();
            }
        }
    }

    void remove(Entity* entity) {
        std::vector<sf::Vector2i> cells = calculateCells(entity);

        for (const auto& cell : cells) {
            if (_grid.find(cell) != _grid.end()) {
                auto& cellEntities = _grid[cell];
                cellEntities.erase(std::remove(cellEntities.begin(), cellEntities.end(), entity),
                                   cellEntities.end());
                _size--;
            }
        }
    }

    void clear()
    {
        _grid.clear();
    }

    std::vector<Entity *> retrieve(Entity *e) {
        std::vector<sf::Vector2i> cells = calculateCells(e);
        std::vector<Entity *> result;

        for (const auto& cell : cells) {
            if (_grid.find(cell) != _grid.end()) {
                result.insert(result.end(), _grid[cell].begin(), _grid[cell].end());
            }
        }

        return result;
    }

    HashGrid *instance()
    {
        return this;
    }

private:
    struct VectorHash {
        std::size_t operator()(const sf::Vector2i& v) const {
            // You can customize the hash function based on your needs
            return std::hash<float>()(v.x) ^ std::hash<float>()(v.y);
        }
    };

    std::vector<sf::Vector2i> calculateCells(Entity *e) {
        if (!e)
            return std::vector<sf::Vector2i>();
        auto transform = e->getComponent<Transform2D>();
        auto sprite = e->getComponent<Sprite>();

        if (!transform || !sprite)
            return std::vector<sf::Vector2i>();
        int cell_x = static_cast<int>(transform->position.x / _cellSize);
        int cell_y = static_cast<int>(transform->position.y / _cellSize);

        int cell_width = static_cast<int>(sprite->getTexture()->getSize().x / _cellSize ) + 1;
        int cell_height = static_cast<int>(sprite->getTexture()->getSize().y / _cellSize ) + 1;

        std::vector<sf::Vector2i> cells;
        for (int x = cell_x; x < cell_x + cell_width; x++) {
            for (int y = cell_y; y < cell_y + cell_height; y++) {
                cells.emplace_back(x, y);
            }
        }
        return cells;
    }

    void resize() {
        _capacity *= 2;
        std::unordered_map<sf::Vector2i, std::vector<Entity *>, VectorHash> newGrid;
        for (auto& entry : _grid) {
            for (Entity *entity : entry.second) {
                insert(entity);
            }
        }
        _grid = newGrid;
    }


private:
    int _size = 0;
    int _capacity = 100;
    float _cellSize = 1.0f;

    std::unordered_map<sf::Vector2i, std::vector<Entity *>, VectorHash> _grid;
};
