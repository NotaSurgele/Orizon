#include <imgui.h>
#include <imgui-SFML.h>
#include "Engine/Core.hpp"

Core::Core(std::string const& name, std::size_t width, std::size_t height) :
                                                _window(name, width, height),
                                                _input()
{
    _r_manager = RessourcesManager();
    _time = Time();
    instance = this;
    _texture.create(800, 600);
    font.loadFromFile("../assets/LEMONMILK-Regular.otf"); // Load a font
    fpsText.setFont(font);
    fpsText.setCharacterSize(11);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(-10, -10);
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
    fpsText.setString("FPS :" + std::to_string(static_cast<int>(Core::fps)));
}

void Core::run()
{
    Input input = Input();


    ImGui::SFML::Init(WindowInstance.getSFMLRenderWindow());
    start();
    while (isOpen()) {

        _time.start();
        sf::Event event;

        inputHandler(event);
        ImGui::SFML::Update(_window.getSFMLRenderWindow(), _time.getClock().getElapsedTime());
        //ImGui::ShowDemoWindow();

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();
        render();
        _system_handler.systems();
        ImGui::SFML::Render(WindowInstance.getSFMLRenderWindow());
        _window.draw(fpsText);
        CoreDisplay();
        _time.end();
        fpsCalculation();
    }
    destroy();
    _window.close();
}
