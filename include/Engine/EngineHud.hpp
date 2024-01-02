#pragma once
#include "Entity.hpp"
#include "TileMap.hpp"
#include "json.hpp"
#include <vector>
#include <thread>
#include <queue>
#include <ostream>
#include <sstream>
#include <imgui.h>
#include <imgui-SFML.h>

#ifndef GUI
#define GUI_ENTITIES_HEIGHT_SIZE_RATIO 0.40f
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
    void setCurrentSceneFilepath(const std::string& sceneFilepath);
    void currentSceneContent(const nlohmann::json& sceneContent);

    void entityWindow(const std::vector<Entity *>& _registry, const std::vector<TileMap *>& tileMap);
    void entityInformation();
    void consoleWindow();
    void scriptEditor(IComponent *component);

    template <class T = std::string, typename ...Args>
    static inline void writeConsole(const std::string& first, Args... args)
    {
        _msg += first;
        writeConsole(args ...);
    }

    template <typename T, typename ...Args>
    static inline void writeConsole(const T& first, Args... args)
    {
        std::string data;

        data = std::to_string(first);
        _msg += data;
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

    class ComponentTreeNodeFactory {
    public:
        ComponentTreeNodeFactory() = default;
        ~ComponentTreeNodeFactory() = default;

        static inline void create(IComponent *component)
        {
            for (auto& elem : _map) {
                auto sigMap = elem.first;
                auto treeNodeBuilder = elem.second;

                if (component->getSignature().find(sigMap) != std::string::npos) {
                    treeNodeBuilder(component);
                    return;
                }
            }
        }

    private:
        static inline void buildTransformTreeNode(IComponent *c)
        {
            auto transform = dynamic_cast<Transform2D *>(c);
            auto x = std::to_string(transform->position.x);
            auto y = std::to_string(transform->position.y);

            // Handle position
            ImGui::Text("Position");
            ImGui::SameLine();
            ImGui::Text("x: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(50);
            ImGui::InputFloat("##posX", &transform->position.x);
            ImGui::SameLine();
            ImGui::Text("y: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(50);
            ImGui::InputFloat("##posY", &transform->position.y);

            // Handle size
            ImGui::Text("Scale");
            ImGui::SameLine();
            ImGui::Text("x: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(50);
            ImGui::InputFloat("##scaleX", &transform->scale.x);
            ImGui::SameLine();
            ImGui::Text("y: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(50);
            ImGui::InputFloat("##scaleY", &transform->scale.y);
        }

    private:
        static inline std::unordered_map<std::string, std::function<void(IComponent *)>> _map= {
            { "Transform2D", buildTransformTreeNode }
        };
    };

private:
    static void insert(const std::string& msg) { _consoleMsg.push(msg); _msg.clear(); }
    void layersEntity(std::size_t& index,  const std::vector<TileMap *>& tileMap);

private:
    std::string _currentSceneFilepath;
    nlohmann::json _currentSceneContent;
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