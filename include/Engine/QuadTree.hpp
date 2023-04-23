#pragma once
#include <vector>
#include <algorithm>
#include "Engine/Entity.hpp"

class QuadtreeNode {
public:
    sf::FloatRect boundingBox;
    std::vector<Entity*> entities;
    bool isLeaf;
    QuadtreeNode* childNodes[4];
    sf::RectangleShape rect;

    QuadtreeNode(const sf::FloatRect& boundingBox);
    ~QuadtreeNode();

    void insert(Entity* entity);
    void retrieve(Entity* entity, std::vector<Entity*>& foundEntities);
    void clear();
    
private:
    void split();
};

class Quadtree {
public:
    Quadtree(const sf::FloatRect& boundingBox);

    void insert(Entity* entity);
    void remove(Entity* entity);
    void clear();
    void setBoundingBox(const sf::FloatRect& newShape);
    std::vector<Entity*> retrieve(Entity* entity);

private:
    sf::RectangleShape rect;
    QuadtreeNode root;
};
