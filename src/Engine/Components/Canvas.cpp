#include "Canvas.hpp"
#include "System.hpp"
#include "Core.hpp"

Canvas::Canvas(Entity *e) : _e(e)
{
    System::__addCanvas(e);
    _font.loadFromFile("../assets/LEMONMILK-Regular.otf");
}

Text *Canvas::addText(const std::string &content, const sf::Vector2f &pos, const std::size_t &size, bool save)
{
    auto *newText = new Text;

    newText->setString(content);
    newText->setCharacterSize(size);
    newText->setPosition(pos);
    newText->setFont(_font);
    newText->setSave(save);
    _text.emplace(newText, pos);
    return newText;
}

Button *Canvas::addButton(const sf::Vector2f& position, const sf::Vector2f& scale, sf::Texture texture, bool save)
{
    auto *button = new Button(position, scale, texture);

    button->setTexture(texture);
    button->setSave(save);
    button->setBasePosition(position);
    _button.push_back(button);
    return button;
}

void Canvas::destroy()
{
    System::__removeCanvas(_e);
    for (auto& t : _text) {
        delete t.first;
    }

    for (auto& b : _button) {
        delete b;
    }
    _button.clear();
    _text.clear();
}

// Button definitions
Button::Button(const sf::Vector2f &position, const sf::Vector2f &size, sf::Texture texture) :
                                                                        _position(position),
                                                                        _size(size),
                                                                        _sprite()
{
    _sprite = new Sprite(texture);

    _sprite->setPosition(position);
    _sprite->setScale(_size.x, _size.y);
}

bool Button::isHovered()
{
    return state == Button::States::HOVERED;
}

bool Button::isClicked()
{
    return isHovered() && Input::isButtonReleased("Left");
}