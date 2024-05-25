#pragma once
#include "SFML/Graphics.hpp"

class Line {
public:
    Line(float x1, float y1, float x2,
         float y2, float thickness=1.0f, sf::Color color=sf::Color::White);
    ~Line() = default;

    void setColor(const sf::Color& color);
    void setEndPoint(float x2, float y2);
    void setStartPoint(float x1, float y1);
    void setStartPoint(const sf::Vector2f& point);
    void setEndPoint(const sf::Vector2f& point);
    void setThickness(float thickness);

    sf::Vector2f getStartPoint();
    sf::Vector2f getEndPoint();
    void draw();

private:
    float _thickness = 1.0f;
    sf::VertexArray _line;
    sf::Vector2f _endPosition = sf::Vector2f(0, 0);
};