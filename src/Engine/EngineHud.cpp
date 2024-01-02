#include "EngineHud.hpp"
#include "Tag.hpp"
#include "Utils.hpp"
#include "Core.hpp"
#include <imgui.h>
#include <imgui-SFML.h>
#define GUI
#include "Script.hpp"

void EngineHud::setTheme()
{
    if (_theme)
        return;
    ImGuiStyle *style = &ImGui::GetStyle();
    ImVec4 *colors = style->Colors;

    colors[ImGuiCol_WindowBg] = ImVec4(0.54, 0.56, 0.56, .5f);
    _theme = true;
}

void EngineHud::entityWindow(const std::vector<Entity *>& _registry, const std::vector<TileMap *>& tileMap)
{
    //_selected = nullptr;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(_height * GUI_ENTITIES_HEIGHT_SIZE_RATIO,
                                    _width * GUI_ENTITIES_WIDTH_SIZE_RATIO), ImGuiCond_FirstUseEver);
    ImGui::Begin("Entities");
    std::size_t index = 1;
    /* Process entity that are not tiles */
    for (const auto& e : _registry) {
        if (!e) continue;
        bool contain = false;

        for (auto& layer : tileMap) {
            if (layer->isInside(e)) {
                contain = true;
                break;
            }
        }
        if (contain)
            continue;
        auto tag = e->getComponent<Tag>();
        std::string name = "Entity " + std::to_string(index);
        if (tag)
            name = tag->value();
        if (ImGui::Selectable(name.c_str())) {
            _selected = e;
        }
        index++;
    }
    /* Handle tiles */
    layersEntity(index, tileMap);
    ImGui::End();
}

void EngineHud::layersEntity(std::size_t& index, const std::vector<TileMap *>& tileMap)
{
    std::size_t layerIndex = 1;
    for (const auto& layer : tileMap) {
        std::string layerName = "Layer " + std::to_string(layerIndex);
        if (ImGui::TreeNode(layerName.c_str())) {
            for (auto& e : layer->getAllEntities()) {
                auto tag = e->getComponent<Tag>();
                std::string name = "Entity " + std::to_string(index);
                if (tag)
                    name = tag->value();
                if (ImGui::Selectable(name.c_str())) {
                    _selected = e;
                }
                index++;
            }
            ImGui::TreePop();
        }
        layerIndex++;
    }
}

void EngineHud::entityInformation()
{
    ImGui::SetNextWindowPos(ImVec2(_width - (_height * GUI_ENTITIES_HEIGHT_SIZE_RATIO), 0));
    ImGui::SetNextWindowSize(ImVec2(_height * GUI_ENTITIES_HEIGHT_SIZE_RATIO,
                                    _width * GUI_ENTITIES_WIDTH_SIZE_RATIO));
    ImGui::Begin("Entity information");
    if (_selected) {
        auto components = _selected->getComponents();
        std::size_t i = 0;

        for (auto& elem : components) {
            if (elem.second == nullptr) {
                _selected->removeComponent(elem.second);
                continue;
            }
            auto signature = elem.first;
            std::string updated = std::to_string(i) + signature;
            if (ImGui::Selectable(updated.c_str())) {
                _selectedC = elem.second;
            }
            i++;
        }
        if (_selectedC)
            scriptEditor(_selectedC);
    }
    ImGui::End();
}

void EngineHud::consoleWindow()
{
    ImGui::SetNextWindowSize(ImVec2(100, 50), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(_width - (_height * GUI_CONSOLE_HEIGHT_SIZE_RATIO),
                                       _height - (_width * GUI_CONSOLE_WIDTH_SIZE_RATIO)),
                                    ImGuiCond_FirstUseEver);
    ImGui::Begin("Console");
    while (!_consoleMsg.empty()) {
        auto msg = _consoleMsg.front();

        ImGui::Text(msg.c_str());
        _consoleMsg.pop();
    }
    ImGui::Separator();
    ImGui::InputText("##consoleInput", _consoleInputText.data(),
                     4096, ImGuiInputTextFlags_AllowTabInput);
    ImGui::End();
}

void EngineHud::scriptEditor(IComponent *component)
{
    std::string signature = component->getSignature();

    if (signature.find("Script") ==
        std::string::npos)
        return;
    Script *script = dynamic_cast<Script *>(component);
    if (_lastScript != script) {
        _lastScript = script;
        _scriptContent = R_GET_SCRIPT(script->getFile());
    }
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
    ImGui::Begin("File Editor");
    if (ImGui::Button("x")) {
         ImGui::End();
         _selectedC = nullptr;
         return;
    }
    if (ImGui::Button("Save File")) {
        Utils::writeFile(script->getFile(), _scriptContent);
        script->reload();
    }
    ImGui::Separator();
    ImGui::InputTextMultiline("##editor", _scriptContent.data(), _scriptContent.size(),
      ImVec2(-1.0f, -1.0f), ImGuiInputTextFlags_AllowTabInput);
    ImGui::End();
}