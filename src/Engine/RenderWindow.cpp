#include "Engine/RenderWindow.hpp"
#include "config.hpp"
#include "Components/View.hpp"

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

void RenderWindow::setView(View *view)
{
    _view = view;
    _window.setView(*view);
}

View *RenderWindow::getView()
{
    return _view;
}

sf::RenderWindow& RenderWindow::getSFMLRenderWindow()
{
    return _window;
}

void RenderWindow::draw(Drawable *sprite)
{
    if (sprite == nullptr)  {
        if (DEBUG_MESSAGE)
            std::cerr << DRAW_NULL_ERROR << std::endl;
        return;
    }
    _window.draw(static_cast<sf::Drawable &>(*sprite), sf::BlendAlpha);
}

void RenderWindow::draw(sf::Drawable const& draw)
{
    _window.draw(draw);
}

void RenderWindow::close()
{
    _window.close();
}
