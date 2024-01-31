#include "Canvas.hpp"
#include "System.hpp"

Canvas::Canvas(Entity *e) : _e(e)
{
    System::__addCanvas(e);
    _font.loadFromFile("../assets/LEMONMILK-Regular.otf");
}

void Canvas::addText(const std::string &content, const sf::Vector2f &pos, const std::size_t &size,
                                    const sf::Color& color)
{
    sf::Text *newText = new sf::Text;

    newText->setFillColor(color);
    newText->setString(content);
    newText->setCharacterSize(size);
    newText->setPosition(pos);
    newText->setFont(_font);
    _text.push_back(newText);
}

void Canvas::addText(sf::Text *text)
{
    _text.push_back(text);
}

void Canvas::destroy()
{
    System::__removeCanvas(_e);
    for (auto& t : _text) {
        delete t;
    }
    _text.clear();
}