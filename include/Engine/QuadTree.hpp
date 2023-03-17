#pragma once

#include <iostream>
#include <vector>
#include <random>

#include "Components/BoxCollider.hpp"
#include "Components/Velocity.hpp"
#include "Entity.hpp"

class System;

class Rectangle {
public:

    Rectangle(float x, float y, float w, float h) : _x(x),
                                                    _y(y),
                                                    _w(w),
                                                    _h(h)
    {
    }

    ~Rectangle() = default;

    bool contain(const sf::Vector2<float>& point)
    {
        return (
            point.x < _x + _w &&
            point.x > _x      &&
            point.y < _y + _h &&
            point.y > _y
        );
    }

    Rectangle operator=(const Rectangle& rect)
    {
        this->_x = rect._x;
        this->_y = rect._y;
        this->_w = rect._w;
        this->_h = rect._h;
        return *this;
    }

public:
    float _x = 0;
    float _y = 0;
    float _w = 0;
    float _h = 0;
};

class QuadTree {
public:
    QuadTree(const Rectangle& bounds, const int& max, const std::string& type) : _r(bounds),
                                                                                _max(max),
                                                                                _type(type)
    {
    }

    ~QuadTree()
    {
        for (auto &quad : _quads) {
            if (quad)
                quad->destroy();
        }
    }

    bool insert(Entity *e)
    {
        auto point = e->getComponent<Transform2D>()->position;

        if (!_r.contain(point))
            return false;
        if (_size < _max) {
            _points.push_back(e);
            _size++;
            return true;
        } else {
            if (!_subdivide) {
                subdivide();
                _subdivide = true;
            }
        }
        for (auto& it : _quads) {
            if (it != nullptr) {
                if (it->insert(e))
                    return true;
            }
        }
        return false;
    }

    void subdivide()
    {
        const float x = _r._x;
        const float y = _r._y;
        const float w = _r._w;
        const float h = _r._h;
        const Rectangle tl = { x , y + (h / 2), w / 2, h / 2 };
        const Rectangle tr = { x + (w / 2), y + (h / 2), w / 2, h / 2 };
        const Rectangle bl = { x, y, w / 2, h / 2 };
        const Rectangle br = { x + (w / 2), y, w / 2, h / 2 };
        _quads[0] = new QuadTree(tl, _max, "top left");
        _quads[1] = new QuadTree(tr, _max, "top right");
        _quads[2] = new QuadTree(bl, _max, "bot left");
        _quads[3] = new QuadTree(br, _max, "bot right");
        // std::cout << "Top Left Rect " << tl._x << " " << tl._y << " " << tl._w << " " << tl._h << std::endl;
        // std::cout << "Top Right Rect " << tr._x << " " << tr._y << " " << tr._w << " " << tr._h << std::endl;
        // std::cout << "Bot Left Rect " << bl._x << " " << bl._y << " " << bl._w << " " << bl._h << std::endl;
        // std::cout << "Bot Right Rect " << br._x << " " << br._y << " " << br._w << " " << br._h << std::endl;
    }

    void collide(System& system);

    void setNewPos(const Rectangle& r)
    {
        _r = r;
    }

    void destroy()
    {
        if (_points.size() > 0) {
            _points.clear();
            _size = 0;
            _subdivide = false;
        }
        for (auto &quad : _quads) {
            if (quad) {
                delete quad;
                quad = nullptr;
            }
        }
    }

    void show();

private:
    using QuadArray = std::array<QuadTree *, 4>;

    QuadArray _quads = {0};
    bool _subdivide = false;
    std::vector<Entity *> _points;
    int _size = 0;
    const int _max = 0;
    const std::string _type = "";
    Rectangle _r = {0, 0, 0, 0};
};
