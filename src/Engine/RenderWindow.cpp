#include "Engine/RenderWindow.hpp"
#include "config.hpp"
#include "Components/View.hpp"
#include <imgui.h>
#include <imgui-SFML.h>

RenderWindow::RenderWindow(std::string const& windowName,
                            std::size_t width,
                            std::size_t height) :
                            _width(width),
                            _height(height)

{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    _window = new sf::RenderWindow(sf::VideoMode(width, height), windowName,
                                   sf::Style::Default, settings);
    _window->setFramerateLimit(144);
}

sf::Vector2f RenderWindow::mapPixelToCoords(const sf::Vector2i &point)
{
    return _window->mapPixelToCoords(point);
}

sf::Vector2i RenderWindow::mapCoordsToPixel(const sf::Vector2f &point)
{
    return _window->mapCoordsToPixel(point);
}

void RenderWindow::clear(sf::Color color)
{
    _window->clear(color);
}

bool RenderWindow::isOpen()
{
    return _window->isOpen();
}

bool RenderWindow::pollEvent(sf::Event& event)
{
    return _window->pollEvent(event);
}

void RenderWindow::display()
{
    _window->display();
}

void RenderWindow::setView(View *view)
{
    _view = view;
    _window->setView(*view);
}

void RenderWindow::setActive(bool active)
{
    _window->setActive(active);
}

View *RenderWindow::getView()
{
    return _view;
}

sf::RenderWindow *RenderWindow::getSFMLRenderWindow()
{
    return _window;
}

void RenderWindow::draw(sf::VertexArray& sprite, sf::RenderStates &states)
{
    _window->draw(sprite, states);
}

void RenderWindow::draw(Drawable *sprite)
{
    if (sprite == nullptr)  {
        if (DEBUG_MESSAGE)
            std::cerr << DRAW_NULL_ERROR << std::endl;
        return;
    }
    _window->draw(static_cast<sf::Drawable &>(*sprite), sf::BlendAlpha);
}

void RenderWindow::draw(Drawable *sprite, const sf::BlendMode& blendMode)
{
    if (sprite == nullptr)  {
        if (DEBUG_MESSAGE)
            std::cerr << DRAW_NULL_ERROR << std::endl;
        return;
    }
    _window->draw(static_cast<sf::Drawable &>(*sprite), blendMode);
}


void RenderWindow::draw(sf::Drawable const& draw)
{
    _window->draw(draw);
}

void RenderWindow::draw(sf::Drawable const& draw, const sf::BlendMode& blendMode)
{
    _window->draw(draw, blendMode);
}

void RenderWindow::close()
{
    _window->close();
}
