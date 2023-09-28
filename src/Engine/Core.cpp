#include "Engine/Core.hpp"

Core::Core(std::string const& name, std::size_t width, std::size_t height) :
                                                _window(name, width, height),
                                                _input()
{
    _r_manager = RessourcesManager();
    _time = Time();
    instance = this;
    _texture.create(800, 600);
}

void Core::loadInputFromFile(std::string const& file)
{
    _input.loadFromFile(file);
}

void Core::CoreClear(sf::Color color)
{
    _window.clear(color);
}

bool Core::CoreEvent(sf::Event& event)
{
    return _window.pollEvent(event);
}

void Core::CoreDraw(Drawable *component)
{
    _window.draw(component);
}

void Core::CoreDraw(Drawable *component, const sf::BlendMode& blendMode)
{
    _window.draw(component, blendMode);
}

void Core::CoreDraw(sf::Drawable const& draw)
{
    _window.draw(draw);
}

void Core::CoreDraw(sf::Drawable const& draw, const sf::BlendMode& blendMode)
{
    _window.draw(draw, blendMode);
}

void Core::CoreDisplay()
{
    _window.display();
}

bool Core::isOpen()
{
    return _window.isOpen();
}

RenderWindow& Core::getWindow()
{
    return _window;
}

void Core::CoreClose()
{
    _window.close();
    _r_manager.destroy();
}

void Core::setView(View *view)
{
    _window.setView(view);
}

void Core::run()
{
    Input input = Input();

    start();
    while (isOpen()) {
        _time.update();
        sf::Event event;

        while (CoreEvent(event)) {
            if (event.type == sf::Event::Closed)
                CoreClose();
            if (event.type == sf::Event::KeyPressed)
                _input.___push_key(event.key.code);
            if (event.type == sf::Event::KeyReleased)
                _input.___remove_key(event.key.code);
            if (event.type == sf::Event::MouseButtonPressed)
                _input.___push_button(event.mouseButton.button);
            if (event.type == sf::Event::MouseButtonReleased)
                _input.___remove_button(event.mouseButton.button);
        }
        render();
        _system_handler.systems();
        CoreDisplay();
        float currentTime = _time.getClock().getElapsedTime().asSeconds();
        Core::fps = 1.f / (currentTime - _lastTime);
        _lastTime = currentTime;
    }
    destroy();
    _window.close();
}
