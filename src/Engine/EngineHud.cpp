#include "EngineHud.hpp"
#include "Tag.hpp"
#include "Utils.hpp"
#include "Core.hpp"
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

void EngineHud::setCurrentSceneFilepath(const std::string &sceneFilepath)
{
    _currentSceneFilepath = sceneFilepath;
}

void EngineHud::currentSceneContent(const nlohmann::json &sceneContent)
{
    _currentSceneContent = sceneContent;
}

nlohmann::json EngineHud::ComponentSerializerFactory::serializeTransform2D(IComponent *c)
{
    auto transform = dynamic_cast<Transform2D *>(c);
    nlohmann::json json;

    json["type"] = "Transform2D";
    json["position"] = { transform->position.x, transform->position.y };
    json["rotation"] = transform->rotation;
    json["scale"] = { transform->scale.x, transform->scale.y };
    json["size"] = { transform->size.x, transform->size.y };
    return json;
}

nlohmann::json EngineHud::ComponentSerializerFactory::serializeBoxCollider(IComponent *c)
{
    auto box = dynamic_cast<BoxCollider *>(c);
    nlohmann::json json;

    json["type"] = "BoxCollider";
    json["position"] = { box->getPosition().x, box->getPosition().y };
    json["size"] = { box->getSize().x, box->getSize().y };
    json["range"] = box->getRange();
    json["collision_type"] = box->getType() == BoxCollider::Type::STATIC ? "Static" : "Dynamic";
    return json;
}

nlohmann::json EngineHud::ComponentSerializerFactory::serializeSprite(IComponent *c)
{
    auto sprite = dynamic_cast<Sprite *>(c);
    nlohmann::json json;
    json["type"] = "Sprite";
    json["texture_name"] = RESOURCE_MANAGER().textureToName(sprite->getTexture());
    return json;
}

nlohmann::json EngineHud::ComponentSerializerFactory::serializeVelocity(IComponent *c)
{
    nlohmann::json json;
    json["type"] = "Velocity";
    return json;
}

nlohmann::json EngineHud::ComponentSerializerFactory::serializeAnimation(IComponent *c)
{
    auto animator = dynamic_cast<Animator *>(c);
    nlohmann::json json;

    json["type"] = "Animator";
    for (auto& it : animator->getAnimations()) {
        auto& name = it.first;
        auto animation = it.second;
        auto offset = animation.getOffsetRect();

        json["animations"].push_back({
            { "name", name },
            { "frames_number", animation.getFramesNumber() },
            { "rect",  { offset[0], offset[1], offset[2], offset[3] }},
            { "speed", animation.getSpeed() }
        });
    }
    return json;
}

nlohmann::json EngineHud::ComponentSerializerFactory::serializeView(IComponent *c)
{
    auto view = dynamic_cast<View *>(c);
    nlohmann::json json;
    auto viewPort = view->getViewBounds();

    json["type"] = "View";
    json["viewport"] = { viewPort.left, viewPort.top, viewPort.width, viewPort.height };
    json["follow"] = view->isFollowing();
    return json;
}

nlohmann::json EngineHud::ComponentSerializerFactory::serializeTag(IComponent *c)
{
    auto tag = dynamic_cast<Tag *>(c);
    nlohmann::json json;

    json["type"] = "Tag";
    json["tag"] = tag->value();
    return json;
}

nlohmann::json EngineHud::ComponentSerializerFactory::serializeLayer(IComponent *c)
{
    auto layer = dynamic_cast<Layer *>(c);
    nlohmann::json json;

    json["type"] = "Layer";
    json["value"] = layer->value();
    return json;
}

nlohmann::json EngineHud::ComponentSerializerFactory::serializeSound(IComponent *c)
{
    auto sound = dynamic_cast<Sound *>(c);
    nlohmann::json json;

    json["type"] = "Sound";
    json["sound_name"] = sound->name();
    json["loop"] = sound->isLoop();
    return json;
}

nlohmann::json EngineHud::ComponentSerializerFactory::serializeMusic(IComponent *c)
{
    auto music = dynamic_cast<OrizonMusic *>(c);
    nlohmann::json json;

    json["type"] = "Music";
    json["music_name"] = music->name();
    json["loop"] = music->isLoop();
    return json;
}

nlohmann::json EngineHud::ComponentSerializerFactory::serializeScript(IComponent *c)
{
    auto script = dynamic_cast<Script *>(c);
    nlohmann::json json;

    json["type"] = "Script";
    json["path"] = script->getFile();
    return json;
}

nlohmann::json EngineHud::ComponentSerializerFactory::serializeLight(IComponent *c)
{
    auto light = dynamic_cast<Light *>(c);
    float intensity = light->getIntensity();
    float emission = light->getEmission();
    Sprite *sprite = light->getSprite();
    nlohmann::json json;

    json["type"] = "Light";
    if (intensity != .4f)
        json["intensity"] = intensity;
    if (light->isSpriteLoaded())
        json["texture_name"] = RESOURCE_MANAGER().textureToName(sprite->getTexture());
    json["emission"] = emission;
    return json;
}

nlohmann::json EngineHud::ComponentSerializerFactory::serializeGravity(IComponent *c)
{
    auto gravity = dynamic_cast<Gravity *>(c);
    nlohmann::json json;

    json["type"] = "Gravity";
    json["force"] = gravity->force;
    return json;
}


std::unordered_map<std::string, Entity *> EngineHud::getEntitiesNameToSave(const nlohmann::json &entitiesJson)
{
    // Get all the wanted to saved entity
    std::unordered_map<std::string, Entity *> entitiesName;

    for (auto& s : entitiesJson) {
        for (auto& eSave : _toSave) {
            auto tag = eSave->getComponent<Tag>();

            if (!tag) continue;
            if (tag->value().find(s) != std::string::npos) {
                entitiesName[s] = eSave;
                break;
            }
        }
    }
    return entitiesName;
}

void EngineHud::ComponentTreeNodeFactory::buildTransformTreeNode(IComponent *c)
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

    //Handle rotation
    ImGui::Text("Rotation");
    ImGui::SameLine();
    ImGui::SliderFloat("##Rotation", &transform->rotation, 0, 360, "%2f");
}

void EngineHud::ComponentTreeNodeFactory::buildTagTreeNode(IComponent *c)
{
    auto tag = dynamic_cast<Tag *>(c);
    auto& value = tag->value();

    ImGui::Text("Tag value");
    ImGui::SameLine();
    if (ImGui::Button(value.data())){
        ImGui::SameLine();
        ImGui::OpenPopup("Available tags");
    }

    if (ImGui::BeginPopup("Available tags")) {
        for (auto& it : R_GET_TAGS()) {
            auto& s = it.first;

            if (ImGui::Selectable(s.data())) {
                tag->setValue(s);
                ImGui::CloseCurrentPopup();
                break;
            }
        }
        ImGui::EndPopup();
    }
    ImGui::Text("Register new tag value");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    ImGui::InputText("##tagValue", _newVal.data(), 256);
    ImGui::SameLine();
    if (ImGui::Button("+")) {
        R_ADD_TAG(_newVal);
        _newVal.clear();
        _newVal = "new value";
    }
}

void EngineHud::componentSerializer(nlohmann::json &entityJson, Entity *e)
{
    auto components = e->getComponents();
    entityJson["components"].clear();

    for (auto& it : components) {
        if (it.second != nullptr) {
            auto serializedComponent = ComponentSerializerFactory::serialize(it.second);
            if (!serializedComponent.is_null())
                entityJson["components"].push_back(serializedComponent);
        }
    }
}

void EngineHud::saveScene()
{
    if (Input::isKeyDown("LControl") &&
        Input::isKeyDown("S")) {
        try {

            std::unordered_map<std::string, Entity *> toSaves = getEntitiesNameToSave(_currentSceneContent["entities"]);
            std::string entitiesPath;

            for (auto& r : _currentSceneContent["resources"]) {
                if (r["type"].get<std::string>().find("Entities") != std::string::npos)
                    entitiesPath = r["path"];
            }
            auto entitiesContentJson = Utils::readfileToJson(entitiesPath);

            for (auto &it : toSaves) {
                auto& name = it.first;
                auto *e = it.second;

                for (auto& json : entitiesContentJson["entities"]) {
                    auto entityName = json["name"];
                    if (entityName.get<std::string>().find(name) != std::string::npos) {
                        componentSerializer(json, e);
                    }
                }
            }
            std::string content = entitiesContentJson.dump(4);
            Utils::writeFile(entitiesPath + std::to_string(1), content);
        } catch (std::exception& msg) {
            std::cerr << msg.what() << std::endl;
        }
    }
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
            if (ImGui::TreeNode(updated.c_str())) {
                _selectedC = elem.second;
                ComponentTreeNodeFactory::create(elem.second);
                ImGui::TreePop();
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
        auto& msg = _consoleMsg.front();

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

    if (signature.find("Script") == std::string::npos) return;

    auto *script = dynamic_cast<Script *>(component);
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