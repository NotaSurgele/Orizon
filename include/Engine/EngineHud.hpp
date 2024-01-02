#pragma once
#include "Entity.hpp"
#include "TileMap.hpp"
#include <vector>
#include <thread>
#include <queue>
#include <ostream>
#include <sstream>

#ifndef GUI
#define GUI_ENTITIES_HEIGHT_SIZE_RATIO 0.25f
#define GUI_ENTITIES_WIDTH_SIZE_RATIO 0.2f
#define GUI_CONSOLE_WIDTH_SIZE_RATIO .60f
#define GUI_CONSOLE_HEIGHT_SIZE_RATIO .25f
#endif

class EngineHud {
public:
    EngineHud(const std::size_t& width, const std::size_t& height) : _width(width), _height(height)
    {
    }


    ~EngineHud() = default;

    void setTheme();
    void entityWindow(const std::vector<Entity *>& _registry, const std::vector<TileMap *>& tileMap);
    void entityInformation();
    void consoleWindow();
    void scriptEditor(IComponent *component);

    template <typename T, typename ...Args>
    static inline void writeConsole(const T& first, Args... args)
    {
        std::string data;

        data = std::to_string(first);
        _msg += data;
        writeConsole(args ...);
    }

    template <typename T, typename ...Args>
    static inline void writeConsole(const std::string& first, Args... args)
    {
        _msg += first;
        writeConsole(args ...);
    }

private:
    static inline std::string _msg;

    template <typename T>
    static inline void writeConsole(const T& last)
    {
        std::string data = std::to_string(last);
        _msg += data;
        insert(_msg);
    }

    static inline void writeConsole(const std::string& last)
    {
        _msg += last;
        insert(_msg);
    }

private:
    static void insert(const std::string& msg) { _consoleMsg.push(msg); _msg.clear(); }
    void layersEntity(std::size_t& index,  const std::vector<TileMap *>& tileMap);

private:
    Entity *_selected = nullptr;
    IComponent *_selectedC = nullptr;
    std::size_t _width;
    std::size_t _height;
    bool _theme = false;

    // scripting
    std::string _scriptContent;
    Script *_lastScript = nullptr;

    std::string _consoleInputText;

    static inline std::queue<std::string> _consoleMsg;
};