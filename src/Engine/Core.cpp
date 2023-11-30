#include <imgui.h>
#include <imgui-SFML.h>
#include "Engine/Core.hpp"

Core::Core(std::string const& name, std::size_t width, std::size_t height) :
                                                _window(name, width, height),
                                                _input(),
                                                _gui(width, height)
{
    _r_manager = RessourcesManager();
    _time = Time();
    instance = this;
    font.loadFromFile("../assets/LEMONMILK-Regular.otf"); // Load a font
    fpsText.setFont(font);
    fpsText.setCharacterSize(11);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(-10, -10);
    _hud = sf::View(sf::FloatRect(0, 0, width, height));
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

void Core::inputHandler(sf::Event& event)
{
    while (CoreEvent(event)) {
        ImGui::SFML::ProcessEvent(event);

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
}

void Core::fpsCalculation()
{
    float currentTime = Time::deltaTime;
    Core::fps = 1.f / currentTime;

    auto view = _window.getView();
    if (view) {
        auto center = view->getCenter();
        auto size = view->getSize();

        auto fixedPosition = sf::Vector2f(center.x - (size.x / 2), center.y - (size.y / 2));
        fpsText.setPosition(fixedPosition);
    } else {
        fpsText.setPosition(10, 10);
    }
    if (_fpsTime < .1f) {
        _fpsTime += _time.getClock().getElapsedTime().asSeconds();
        return;
    }
    fpsText.setString("FPS :" + std::to_string(static_cast<int>(Core::fps)));
    _fpsTime = 0.0f;
}

void Core::initGui()
{
    if (ENGINE_MODE)
        ImGui::SFML::Init(WindowInstance.getSFMLRenderWindow());
}

void Core::updateGUI()
{
    if (ENGINE_MODE) {
        ImGui::SFML::Update(_window.getSFMLRenderWindow(), _time.getClock().getElapsedTime());
        _gui.entityWindow(_system_handler.getRegistry(), _system_handler.getTileMaps());
        _gui.entityInformation();
        ImGui::SFML::Render(WindowInstance.getSFMLRenderWindow());
    }
}

void Core::destroyGUI()
{
    if (ENGINE_MODE)
        ImGui::SFML::Shutdown();
}

void Core::run()
{
    Input input = Input();

    initGui();
    start();
    while (isOpen()) {

        _time.start();
        sf::Event event;

        inputHandler(event);

        render();
        _system_handler.systems();
        auto old = WindowInstance.getView();
        WindowInstance.getSFMLRenderWindow().setView(_hud);
        updateGUI();
        if (old) WindowInstance.setView(old);
        _window.draw(fpsText);
        CoreDisplay();
        _time.end();
        fpsCalculation();
    }
    destroyGUI();
    destroy();
    _window.close();
}
