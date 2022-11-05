#pragma once

class GameScene;
#include "Engine/Core.hpp"
#include "Game/Scenes/GameScene.hpp"

#define INPUT_FILE "../assets/input.ini"


class App : public Core {
public:
    App(std::string const& windowName, std::size_t width=800, std::size_t height=600) : Core(windowName, width, height) {}
    App() = delete;
    ~App() = default;

    void start() override final;
    void render() override final;
    void destroy() override final;

    GameScene game_scene;
private:
};
