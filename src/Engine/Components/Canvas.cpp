#include "Canvas.hpp"
#include "System.hpp"
#include "Core.hpp"

Canvas::Canvas(Entity *e) : _e(e)
{
    System::__addCanvas(e);
    _font.loadFromFile("../assets/LEMONMILK-Regular.otf");
}

Text *Canvas::addText(const std::string &content, const sf::Vector2f &pos, const std::size_t &size,
                                                                            const sf::Color& color)
{
    auto *newText = new Text;

    newText->setFillColor(color);
    newText->setString(content);
    newText->setCharacterSize(size);
    newText->setPosition(pos);
    newText->setFont(_font);
    _text.emplace(newText, pos);
    return newText;
}

void Canvas::destroy()
{
    System::__removeCanvas(_e);
    for (auto& t : _text) {
        delete t.first;
    }
    _text.clear();
}