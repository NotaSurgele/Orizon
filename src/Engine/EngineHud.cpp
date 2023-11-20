#include "EngineHud.hpp"
#include "Tag.hpp"
#include <imgui.h>
#include <imgui-SFML.h>

void EngineHud::entityWindow(const std::vector<Entity *>& _registry)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(400, 300));
    ImGui::Begin("Entities");
    for (const auto& e : _registry) {
        auto tag = e->getComponent<Tag>();
        std::string name = "Entity";
        if (tag)
            name = tag->value();
        ImGui::Selectable(name.c_str());
    }
    ImGui::End();
}