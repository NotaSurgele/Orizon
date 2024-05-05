#include <imgui.h>
#include <imgui-SFML.h>
#include "Engine/Core.hpp"

#ifdef ENGINE_GUI
#include "EngineHud.hpp"
    static std::atomic<bool> show = true;
#endif

Core::Core(std::string const& name, std::size_t width, std::size_t height) :
                                                _window(name, width, height),
                                                _input(),
                                                _gui(width, height)
{
#ifdef ENGINE_GUI
    if (!(_isTextureLoaded = _windowTexture.create(1063, 951)))
        std::cerr << "[CORE] Couldn't create RenderTexture" << std::endl;
#endif
    _status = sf::RenderStates();
    _r_manager = ResourcesManager();
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
    _clearColor = color;
}

bool Core::CoreEvent(sf::Event& event)
{
    return _window.pollEvent(event);
}

void Core::CoreDraw(Drawable *component)
{
#ifdef ENGINE_GUI
    if (!_isTextureLoaded) return;
    _windowTexture.draw(*component);
#else
    _window.draw(component);
#endif
}

void Core::CoreDraw(Drawable *component, const sf::BlendMode& blendMode)
{
#ifdef ENGINE_GUI
    if (!_isTextureLoaded) return;
    _windowTexture.draw(*component, blendMode);
#else
    _window.draw(component, blendMode);
#endif
}

void Core::CoreDraw(sf::Drawable const& draw)
{
#ifdef ENGINE_GUI
    if (!_isTextureLoaded) return;
    _windowTexture.draw(draw);
#else
    _window.draw(component, blendMode);
#endif
}

void Core::CoreDraw(sf::Drawable const& draw, const sf::BlendMode& blendMode)
{
#ifdef ENGINE_GUI
    if (!_isTextureLoaded) return;
    _windowTexture.draw(draw, blendMode);
#else
    _window.draw(component, blendMode);
#endif
}

void Core::CoreDrawBatch(Sprite *sprite)
{
    if (!_isTextureLoaded) return;
    for (auto batch : _batches) {
        if (batch->textureId == sprite->getTextureId()) {
            batch->draw(sprite);
            return;
        }
    }
    auto newBatch = new SpriteBatch();
    newBatch->texture = sprite->getTexture();
    newBatch->textureCpy = *sprite->getTexture();
    newBatch->textureId = sprite->getTextureId();
    newBatch->sprite = sprite;
    newBatch->draw(sprite);
    _batches.push_back(newBatch);
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
    if (!WindowInstance.getView()) return;
    auto sfmlWindow = WindowInstance.getSFMLRenderWindow();
    auto viewBounds = sfmlWindow->getViewport(sfmlWindow->getView());
    auto mousePosition = sf::Mouse::getPosition(*WindowInstance.getSFMLRenderWindow());

/*    EngineHud::writeConsole<std::string, bool>("Main view is ", _mainViewSelected);
    EngineHud::writeConsole<std::string, int, std::string, int>("Mouse position ", mousePosition.x, " ", mousePosition.y);*/
    while (CoreEvent(event)) {

#ifdef ENGINE_GUI
        ImGui::SFML::ProcessEvent(event);

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                _mainViewSelected = viewBounds.contains(mousePosition.x, mousePosition.y);
            }
        }
#endif
        if (event.type == sf::Event::Closed)
            CoreClose();
#ifdef ENGINE_GUI
        if (!_mainViewSelected) return;
#endif
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
    if (_fpsTime < 1.0f) {
        _fpsTime += _time.getClock().getElapsedTime().asSeconds();
        return;
    }
    fpsText.setString("FPS :" + std::to_string(static_cast<int>(Core::fps)));
    _fpsTime = 0.0f;
}

void Core::initGui()
{
    _baseView = new View(nullptr, 0, 0, 800, 600);
    SET_VIEW(_baseView);
#ifdef ENGINE_GUI
    ImGui::SFML::Init(*WindowInstance.getSFMLRenderWindow());
#endif
}

void Core::updateGUI()
{
#ifdef ENGINE_GUI
    ImGui::SFML::Update(*_window.getSFMLRenderWindow(), _time.getClock().getElapsedTime());
    _guiThread = std::thread([&] () {
        auto currentScene = _sceneManager.getScene();

        if (Input::isKeyDown("F1")) show = !show;
        if (show) {
            _gui.setTheme();
            _gui.setCurrentSceneFilepath(currentScene->getSceneFilepath());
            _gui.currentSceneContent(currentScene->getSceneContent());
            _gui.entityWindow(_system_handler.
                getRegistry(), _system_handler.getTileMaps());
            _gui.entityInformation();
            _gui.consoleWindow();
            _gui.resourceManager();
            _gui.menuBar();
        }
        if (Input::isKeyPressed("LControl") && Input::isKeyDown("S")) _gui.saveScene();
    });
    if (_guiThread.joinable()) _guiThread.join();
    _gui.gameWindow(_windowTexture);
    ImGui::SFML::Render(*_window.getSFMLRenderWindow());
#endif
}

void Core::destroyGUI()
{
#ifdef ENGINE_GUI
    ImGui::SFML::Shutdown();
#endif
    delete _baseView;
}

void Core::run()
{
    initGui();
    start();
    while (isOpen()) {
        _time.start();
        sf::Event event {};

        inputHandler(event);
#ifdef ENGINE_GUI
        _windowTexture.clear(_clearColor);
        _window.clear(_clearColor);
#else
        _window.clear(_clearColor);
#endif
        _system_handler.systems();
        renderBatch();
        clearBatch();
        render();
#ifdef  ENGINE_GUI
        auto old = WindowInstance.getView();
        _windowTexture.setView(_hud);
        updateGUI();
        if (old) _windowTexture.setView(*old);
        EngineHud::writeConsole<std::string, std::string>("", fpsText.getString());
#else
        _window.draw(fpsText);
#endif
        CoreDisplay();
        _time.end();
        fpsCalculation();
        _input.___clearReleased();
    }
    destroyGUI();
    destroy();
    _window.close();
}

void Core::renderBatch()
{
    for (auto batch : _batches) {
#ifdef ENGINE_GUI
        _windowTexture.draw(*((sf::Drawable *)batch));
#else
        _window.draw(*((sf::Drawable *)batch));
#endif
    }
}

void Core::clearBatch()
{
    for (auto batch : _batches)
        batch->clear();
}
