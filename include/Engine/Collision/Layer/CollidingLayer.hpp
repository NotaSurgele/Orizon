#pragma once

#include "Engine/Entity.hpp"

class CollidingLayer {
public:
    CollidingLayer(const float& x, const float& y, const int& width, const int& height, const int& tileWidth, const int& tileHeight);
    ~CollidingLayer();

    bool emplaceEntity(Entity *e);
    bool contain(Entity *e);
    bool contain(const float& x, const float& y);
    bool removeEntity(const int& x, const int& y);
    sf::Vector2f emptySpot(int range);

    std::vector<Entity *> checkAround(Entity *e, int range);
    void display();

public:
    int w = 0;
    int h = 0;
    int tileWidth = 0;
    int tileHeight = 0;
    float _x = 0;
    float _y = 0;

private:
    std::vector<std::vector<Entity *>> _layer;
};