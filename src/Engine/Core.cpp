#include "Engine/Core.hpp"

Core::Core(std::string const& name, std::size_t width, std::size_t height) :
                                                _window(name, width, height),
                                                _input()
{
    _r_manager = RessourcesManager();
    _time = Time();
    instance = this;
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

void Core::CoreDisplay()
{
    _window.display();
}

bool Core::isOpen()
{
    return _window.isOpen();
}

void Core::CoreClose()
{
    _window.close();
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
        }
        render();
        CoreDisplay();
    }
    destroy();
    _window.close();
}
