#include "Engine/RenderWindow.hpp"

RenderWindow::RenderWindow(std::string const& windowName,
                            std::size_t width,
                            std::size_t height) :
                            _width(width),
                            _height(height),
                            _window(sf::VideoMode(width, height), windowName)
{
}

void RenderWindow::clear(sf::Color color)
{
    _window.clear(color);
}

bool RenderWindow::isOpen()
{
    return _window.isOpen();
}

bool RenderWindow::pollEvent(sf::Event& event)
{
    return _window.pollEvent(event);
}

void RenderWindow::display()
{
    _window.display();
}

void RenderWindow::draw(Drawable *sprite)
{
    if (sprite == nullptr) {
        std::cerr << DRAW_NULL_ERROR << std::endl;
        return;
    }
    _window.draw(static_cast<sf::Drawable &>(*sprite));
}

void RenderWindow::close()
{
    _window.close();
}
