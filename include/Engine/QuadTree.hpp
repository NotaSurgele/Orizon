#pragma once

#include <iostream>
#include <vector>
#include <random>

template <typename T>
class Point {
public:
    Point<T>(const T& x2, const T& y2) : x(x2), y(y2)
    {

    }
    ~Point<T>() = default;

public:
    const T x = 0;
    const T y = 0;
};

class Rectangle {
public:
    Rectangle(float x, float y, float w, float h) : _x(x),
                                                    _y(y),
                                                    _w(w),
                                                    _h(h)
    {
    }

    ~Rectangle() = default;

    bool contain(const Point<float>& point)
    {
        return (
            point.x > _x - _w &&
            point.x < _x + _w &&
            point.y > _y - _h &&
            point.y < _y + _h
        );
    } 

public:
    const float _x = 0;
    const float _y = 0;
    const float _w = 0;
    const float _h = 0;
};

class QuadTree {
public:
    QuadTree(const Rectangle& bounds, const int& max, const std::string& type) : _r(bounds),
                                                                                _max(max),
                                                                                _type(type)
    {
    }

    ~QuadTree() = default;

    template <typename T>
    int insert(Point<T> const& point)
    {
        if (!_r.contain(point)) {
            return -1;
        }
        if (_size < _max) {
            _points.push_back(point);
            _size++;
            return 1;
        } else {
            if (!_subdivide) {
                subdivide();
                _subdivide = true;
            }
        }
        _topLeft->insert(point);
        _topRight->insert(point);
        _botLeft->insert(point);
        _botRight->insert(point);
        return 0;
    }

    void subdivide()
    {
        const float x = _r._x;
        const float y = _r._y;
        const float w = _r._w;
        const float h = _r._h;
        const Rectangle tl = {x + w / 2, y - h / 2, w / 2, h / 2};
        const Rectangle tr = {x - w / 2, y - h / 2, w / 2, h / 2};
        const Rectangle bl = {x + w / 2, y + h / 2, w / 2, h / 2};
        const Rectangle br = {x - w / 2, y + h / 2, w / 2, h / 2};
        _topLeft = new QuadTree(tl, _max, "top left");
        _topRight = new QuadTree(tr, _max, "top right");
        _botLeft = new QuadTree(bl, _max, "bot left");
        _botRight = new QuadTree(br, _max, "bot right");
    }

    void show()
    {
        std::cout << "Type: " << _type << std::endl;
        std::cout << "  Rectangle: " << _r._x << ", " << _r._y << " " << _r._w << ", " << _r._h << std::endl;
        for (auto& p : _points)
            std::cout << "      Point: " << p.x << " " << p.y << std::endl;
        if (_topLeft)
            _topLeft->show();
        if (_topRight)
            _topRight->show();
        if (_botLeft)
            _botLeft->show();
        if (_botRight)
            _botRight->show();
    }

private:
    QuadTree *_topLeft = nullptr;
    QuadTree *_topRight = nullptr;
    QuadTree *_botLeft = nullptr;
    QuadTree *_botRight = nullptr;
    bool _subdivide = false;
    std::vector<Point<float>> _points;
    int _size = 0;
    const int _max = 0;
    const std::string _type = "";
    Rectangle _r = {0, 0, 0, 0};
};

