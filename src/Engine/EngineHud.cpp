#include "EngineHud.hpp"
#include "Tag.hpp"
#include <imgui.h>
#include <imgui-SFML.h>

void EngineHud::entityWindow(const std::vector<Entity *>& _registry, const std::vector<TileMap *>& tileMap)
{
    //_selected = nullptr;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(_height * 0.42f, _width * 0.3f));
    ImGui::Begin("Entities");
    std::size_t index = 1;

    /* process entity that are not tiles */
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
    ImGui::End();
}

void EngineHud::entityInformation()
{
    ImGui::SetNextWindowPos(ImVec2(_width - (_height * 0.42f), 0));
    ImGui::SetNextWindowSize(ImVec2(_height * 0.42f, _width * 0.3f));
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