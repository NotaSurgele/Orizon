#pragma once
#include <vector>
#include <algorithm>
#include "Engine/Entity.hpp"

class QuadTree {
public:
    QuadTree(int level, float x, float y, float width, float height, int capacity);
    ~QuadTree();
    void clear();
    void insert(Entity* entity);
    void remove(Entity* entity);
    std::vector<Entity*> retrieve(Entity* entity);
private:
    const int MAX_LEVELS = 5;
    const int MAX_ENTITIES = 10;
    int m_level;
    sf::FloatRect m_bounds;
    int m_capacity;
    std::vector<Entity*> m_entities;
    QuadTree* m_nodes[4];
    void split();
    int getQuadrant(float x, float y);
};
