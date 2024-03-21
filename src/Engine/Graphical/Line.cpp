#include "Line.hpp"
#include "Core.hpp"

Line::Line(float x1, float y1, float x2,
           float y2, float thickness, sf::Color color) : _thickness(thickness),
                                                        _line(sf::TriangleStrip, 4)
{
    // start point
    _line[0].position.x = x1;
    _line[0].position.y = y1;
    _line[1].position.x = x1;
    _line[1].position.y = y1 + thickness;

    //endpoint
    _line[2].position.x = x2;
    _line[2].position.y = y2;

    _line[3].position.x = x2;
    _line[3].position.y = y2 + thickness;

    for (std::size_t index = 0; index < 4; index++) {
        _line[index].color = color;
    }
}

void Line::setColor(const sf::Color& color)
{
    for (std::size_t index = 0; index < 4; index++) {
        _line[index].color = color;
    }
}

void Line::setEndPoint(float x2, float y2)
{
    _line[2].position.x = x2;
    _line[2].position.y = y2;
    _line[3].position.x = x2;
    _line[3].position.y = y2 + _thickness;
}

void Line::setEndPoint(const sf::Vector2f &point)
{
    _line[2].position = point;
    _line[3].position.x = point.x;
    _line[3].position.y = point.y + _thickness;
}

void Line::setStartPoint(const sf::Vector2f &point)
{
    _line[0].position = point;
    _line[1].position.x = point.x;
    _line[1].position.y = point.y + _thickness;
}

void Line::setStartPoint(float x1, float y1)
{
    _line[0].position.x = x1;
    _line[0].position.y = y1;
    _line[1].position.x  = x1;
    _line[1].position.y = y1 + _thickness;
}

void Line::setThickness(float thickness)
{
    _thickness = thickness;
    setStartPoint(_line[0].position);
    setEndPoint(_line[2].position);
}

sf::Vector2f Line::getStartPoint()
{
    return _line[0].position;
}

sf::Vector2f Line::getEndPoint()
{
    return _line[2].position;
}

void Line::draw()
{
    DRAW(_line);
}