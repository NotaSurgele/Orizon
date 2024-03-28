#pragma once
#include <unordered_map>
#include "Entity.hpp"
#include "Components/Sprite.hpp"
#include "EngineHud.hpp"

class HashGrid {
public:

    HashGrid(float const& cellSize=100, int capacity=100) : _size(0),
                                                              _cellSize(cellSize),
                                                              _capacity(capacity) {
    }

    ~HashGrid()
    {
        _grid.clear();
    }

    void insert(Entity* entity) {
        //EngineHud::writeConsole<std::string, std::size_t>("Cell size ", _grid.size());
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
/*        _capacity=100;
        _grid.reserve(_capacity);*/
        _grid.clear();
    }

    std::list<Entity *> retrieve(Entity *e) {
        std::vector<sf::Vector2i> cells = calculateCells(e);
        std::list<Entity *> result;

        EngineHud::writeConsole<std::string, std::size_t>("Cells ", cells.size());
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
            return std::hash<float>()(v.x) ^ std::hash<float>()(v.y);
        }
    };

    std::vector<sf::Vector2i> calculateCells(Entity *e) {
        if (!e) return {};
        auto boxArray = e->getComponents<BoxCollider>();
        std::vector<sf::Vector2i> cells;

        for (auto& box : boxArray) {
            if (!box)
                return {};
            int cell_x = static_cast<int>(box->getPosition().x / _cellSize);
            int cell_y = static_cast<int>(box->getPosition().y / _cellSize);

            int cell_width = static_cast<int>(box->getSize().x / _cellSize ) + 1;
            int cell_height = static_cast<int>(box->getSize().y / _cellSize ) + 1;

            for (int x = cell_x; x < cell_x + cell_width; x++) {
                for (int y = cell_y; y < cell_y + cell_height; y++) {
                    cells.emplace_back(x, y);
                }
            }
        }
        return cells;
    }

    void resize() {
        _capacity *= 2;
    }


private:
    int _size = 0;
    int _capacity = 100;
    float _cellSize = 1.0f;

    std::unordered_map<sf::Vector2i, std::list<Entity *>, VectorHash> _grid;
};
