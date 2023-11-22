#include "EngineHud.hpp"
#include "Tag.hpp"
#include <imgui.h>
#include <imgui-SFML.h>
#define GUI

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
    ImGui::Begin("Entity informations");
    if (_selected) {
        auto components = _selected->getComponents();

        for (auto& elem : components) {
            auto signature = elem.first;

            ImGui::Text(signature);
        }
    }
    ImGui::End();
}