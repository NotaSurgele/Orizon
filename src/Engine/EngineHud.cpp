#include "EngineHud.hpp"
#include "Tag.hpp"
#include "Utils.hpp"
#include "Core.hpp"
#define GUI
#include "Script.hpp"

static int id = 0;

void EngineHud::setTheme()
{
    if (_theme)
        return;
    ImGuiStyle *style = &ImGui::GetStyle();
    ImVec4 *colors = style->Colors;

    colors[ImGuiCol_WindowBg] = ImVec4(0.05, 0.18, 0.18, .5f);
    colors[ImGuiCol_Text] = ImVec4(0.61, 0.8, 0.57, 1.0);
    colors[ImGuiCol_FrameBg] = ImVec4(0.3, 0.3, 0.3, 1.0);
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

void EngineHud::ComponentCreationFactory::createTransform2D(Entity *e)
{
    auto transform = e->getComponent<Transform2D>();

    if (transform) return;
    e->addComponent<Transform2D>();
}

void EngineHud::ComponentCreationFactory::createBoxCollider(Entity *e)
{
    auto transform = e->getComponent<Transform2D>();
    e->addComponent<BoxCollider>(sf::Vector2f(0, 0), transform->size, 1)->setType(BoxCollider::DYNAMIC);
}

void EngineHud::ComponentCreationFactory::createSprite(Entity *e)
{
    auto sprite = e->getComponent<Sprite>();

    if (sprite) return;
    sf::Image blankImage;
    blankImage.create(16, 16, sf::Color::White);

    sf::Texture texture;
    texture.loadFromImage(blankImage);
    e->addComponent<Sprite>(texture, 1, 1)->setTextureName("Default");
}

void EngineHud::ComponentCreationFactory::createVelocity(Entity *e)
{
    auto velocity = e->getComponent<Velocity>();

    if (velocity) return;
    e->addComponent<Velocity>();
}

void EngineHud::ComponentCreationFactory::createAnimator(Entity *e)
{
    auto animator = e->getComponent<Animator>();

    if (animator) return;
    e->addComponent<Animator>();
}

void EngineHud::ComponentCreationFactory::createView(Entity *e)
{
    auto view = e->getComponent<View>();

    if (view) return;
    e->addComponent<View>(0, 0, 400, 300);
}

void EngineHud::ComponentCreationFactory::createTag(Entity *e)
{
    auto tag = e->getComponent<Tag>();

    if (tag) return;
    e->addComponent<Tag>("entity " + std::to_string(e->get_id()));
}

void EngineHud::ComponentCreationFactory::createLayer(Entity *e)
{
    auto layer = e->getComponent<Layer>();

    if (layer) return;
    e->addComponent<Layer>(0);
}

void EngineHud::ComponentCreationFactory::createSound(Entity *e)
{
    e->addComponent<Sound>("");
}

void EngineHud::ComponentCreationFactory::createMusic(Entity *e)
{
    e->addComponent<OrizonMusic>("");
}

void EngineHud::ComponentCreationFactory::createScript(Entity *e)
{
    e->addComponent<Script>("");
    System::__registerScriptedEntity(e);
}

void EngineHud::ComponentCreationFactory::createLight(Entity *e)
{
    sf::Image blankImage;
    blankImage.create(16, 16, sf::Color::White);

    sf::Texture texture;
    texture.loadFromImage(blankImage);
    auto *s  = new Sprite(texture);
    s->setTextureName("Default");
    e->addComponent<Light>(s);
}

void EngineHud::ComponentCreationFactory::createGravity(Entity *e)
{
    auto gravity = e->getComponent<Gravity>();

    if (gravity) return;
    e->addComponent<Gravity>(0.0f);
}

void EngineHud::ComponentCreationFactory::createCanvas(Entity *e)
{
    e->getComponent<Canvas>();
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

    if (sprite->getTextureName().empty()) json["texture_name"] = RESOURCE_MANAGER().textureToName(sprite->getTexture());
    else json["texture_name"] = sprite->getTextureName();
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
    for (auto& animation : animator->getAnimations()) {
        auto name = animation.name();
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
    auto viewBounds = view->getViewBounds();
    auto viewPort = view->getViewport();

    viewPort.left = (viewPort.left - EDITOR_VIEW_SIZE_RATIO) < 0 ? 0 : viewPort.left - EDITOR_VIEW_SIZE_RATIO;
    viewPort.top = (viewPort.top - EDITOR_VIEW_SIZE_RATIO) < 0 ? 0 : viewPort.top - EDITOR_VIEW_SIZE_RATIO;
    json["type"] = "View";
    json["view_bounds"] = { viewBounds.left, viewBounds.top, viewBounds.width * 2, viewBounds.height * 2 };
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
    Sprite *sprite = light->getSprite();
    nlohmann::json json;

    json["type"] = "Light";
    json["intensity"] = intensity;
    json["texture_name"] = RESOURCE_MANAGER().textureToName(sprite->getTexture());
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

nlohmann::json EngineHud::ComponentSerializerFactory::serializeCanvas(IComponent *c)
{
    auto canvas = dynamic_cast<Canvas *>(c);
    auto buttons = canvas->getButtons();
    auto texts = canvas->getTexts();
    auto images = canvas->getImages();
    nlohmann::json json;

    json["type"] = "Canvas";
    json["canvas_objects"] = nlohmann::json::array();
    for (auto &it : buttons) {
        auto& b = it.first;
        auto& offset = it.second;
        auto sprite = b->getSprite();
        auto size = b->getSize();
        auto content = b->getTextContent();
        std::string textureName = RESOURCE_MANAGER().textureToName(sprite->getTexture());
        auto coordType = b->coordType;

        json["canvas_objects"].push_back({
            { "type", "Button" },
            { "position", { offset.x, offset.y } },
            { "size", { size.x, size.y } },
            { "texture_name", textureName },
            { "content", content },
            { "coord_type", coordType }
        });
    }

    for (auto &it : texts) {
        auto& t = it.first;
        auto& offset = it.second;
        auto size = t->getCharacterSize();
        auto content = t->getString();
        auto coordType = t->coordType;

        json["canvas_objects"].push_back({
            { "type", "Text" },
            { "position", { offset.x, offset.y } },
            { "font_size", size },
            { "content", content },
            { "coord_type", coordType }
        });
    }

    for (auto &it : images) {
        auto& i = it.first;
        auto& offset = it.second;
        auto sprite = i->getImage();
        auto size = sprite->getScale();
        std::string textureName = RESOURCE_MANAGER().textureToName(sprite->getTexture());
        auto coordType = i->coordType;

        json["canvas_objects"].push_back({
            { "type", "Image" },
            { "position", { offset.x, offset.y } },
            { "size", { size.x, size.y } },
            { "texture_name", textureName },
            { "coord_type", coordType }
        });
    }
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

void EngineHud::ComponentTreeNodeFactory::buildViewTreeNode(IComponent *c)
{
    auto view = dynamic_cast<View *>(c);
    auto bounds = view->getViewBounds();
    auto position = view->getCenter();

    // Handle position
    ImGui::Text("Center position");
    ImGui::SameLine();
    ImGui::Text("x: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##vposX", &position.x);
    ImGui::SameLine();
    ImGui::Text("y: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##vposY", &position.y);

    // Handle size
    ImGui::Text("Size");
    ImGui::SameLine();
    ImGui::Text("x: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##vscaleX", &bounds.width);
    ImGui::SameLine();
    ImGui::Text("y: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##vscaleY", &bounds.height);

    // Following Checkbox
    ImGui::Checkbox("Follow entity", &view->isFollowing());

    bounds.left = position.x;
    bounds.top = position.y;
    view->setViewBounds(bounds);
}

void EngineHud::ComponentTreeNodeFactory::buildBoxColliderTreeNode(IComponent *c)
{
    auto box = dynamic_cast<BoxCollider *>(c);
    auto& position = box->getOffset();
    auto& size = box->getSize();
    bool draw = box->shouldDraw();

    // Handle position
    ImGui::Text("Position");
    ImGui::SameLine();
    ImGui::Text("x: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##posX", &position.x);
    ImGui::SameLine();
    ImGui::Text("y: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##posY", &position.y);

    // Handle size
    ImGui::Text("Size");
    ImGui::SameLine();
    ImGui::Text("x: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##scaleX", &size.x);
    ImGui::SameLine();
    ImGui::Text("y: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##scaleY", &size.y);

    // Following Checkbox
    box->setOffset(position);
    box->setSize(size);
    ImGui::Checkbox("Draw box", &draw);
    box->draw(draw);
}

void EngineHud::ComponentTreeNodeFactory::buildVelocityTreeNode(IComponent *c)
{
    auto velocity = dynamic_cast<Velocity *>(c);
    auto vector = sf::Vector2<float>(velocity->getX(), velocity->getY());

    // Handle position
    ImGui::Text("Value");
    ImGui::SameLine();
    ImGui::Text("x: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##posX", &vector.x);
    ImGui::SameLine();
    ImGui::Text("y: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##posY", &vector.y);

    velocity->setX(vector.x);
    velocity->setY(vector.y);
}

void EngineHud::ComponentTreeNodeFactory::buildSoundTreeNode(IComponent *c)
{
    auto sound = dynamic_cast<Sound *>(c);
    float volume = sound->getVolume();
    std::string name = sound->name();
    bool loop = sound->isLoop();

    if (name.empty()) name = "Default";
    ImGui::InputFloat("Volume", &volume);
    ImGui::Text("Sound buffer");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    if (ImGui::Button(name.data())) {
        ImGui::SameLine();
        ImGui::OpenPopup("Sound buffers");
    }
    if (ImGui::BeginPopup("Sound buffers")) {
        for (auto& it : R_GET_RESSOURCES(sf::SoundBuffer)) {
            auto& s = it.first;

            if (ImGui::Selectable(s.data())) {
                sound->setBuffer(it.second);
                sound->setName(s);
                ImGui::CloseCurrentPopup();
                break;
            }
        }
        ImGui::EndPopup();
    }
    ImGui::Checkbox("Loop", &loop);
    ImGui::SameLine();
    if (ImGui::SmallButton("Play")) sound->play();
    ImGui::SameLine();
    if (ImGui::SmallButton("Stop")) sound->stop();
    sound->setLoop(loop);
    sound->setVolume(volume);
}

void EngineHud::ComponentTreeNodeFactory::buildLayerTreeNode(IComponent *c)
{
    auto layer = dynamic_cast<Layer *>(c);
    int value = (int)layer->value();

    ImGui::InputInt("Draw order", &value);
    layer->set((std::size_t)value);
}

void EngineHud::ComponentTreeNodeFactory::buildOrizonMusicTreeNode(IComponent *c)
{
    auto music = dynamic_cast<OrizonMusic *>(c);
    float volume = music->getVolume();
    std::string name = music->name();
    bool loop = music->isLoop();

    if (name.empty()) name = "Default";
    ImGui::InputFloat("Volume", &volume);
    ImGui::Text("Music buffer");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    if (ImGui::Button(name.data())) {
        ImGui::SameLine();
        ImGui::OpenPopup("Music buffers");
    }
    if (ImGui::BeginPopup("Music buffers")) {
        for (auto& it : R_GET_MUSICS()) {
            auto& s = it.first;

            if (ImGui::Selectable(s.data())) {
                music->setMusic(it.second);
                music->setName(s);
                ImGui::CloseCurrentPopup();
                break;
            }
        }
        ImGui::EndPopup();
    }
    ImGui::Checkbox("Loop", &loop);
    ImGui::SameLine();
    if (ImGui::SmallButton("Play")) music->play();
    ImGui::SameLine();
    if (ImGui::SmallButton("Stop")) music->stop();
    music->setLoop(loop);
    music->setVolume(volume);
}

void EngineHud::ComponentTreeNodeFactory::buildScriptTreeNode(IComponent *c)
{
    auto script = dynamic_cast<Script *>(c);
    auto path = script->getFile();

    if (path.empty()) path = "Default";
    ImGui::Text("Current script: ");
    ImGui::SameLine();
    if (ImGui::Button(path.data())) {
        ImGui::SameLine();
        ImGui::OpenPopup("Script files");
    }
    if (ImGui::BeginPopup("Script files")) {
        for (auto& it : R_GET_SCRIPTS()) {
            auto s = it.first;

            if (s.find(path) != std::string::npos) continue;
            if (ImGui::Selectable(s.data())) {
                script->setScript(s);
                ImGui::CloseCurrentPopup();
                break;
            }
        }
        ImGui::EndPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("View")) _scriptWindow = true;
    if (_scriptWindow) scriptEditor(script);
}

void EngineHud::ComponentTreeNodeFactory::buildSpriteTreeNode(IComponent *c)
{
    auto sprite = dynamic_cast<Sprite *>(c);
    auto color = sprite->getColor();
    auto texture = sprite->getTexture();
    uint8_t colorArr[4] = { color.r, color.g, color.b, color.a };
    std::string& name = sprite->getTextureName();

    if (name.empty()) {
        name = RESOURCE_MANAGER().textureToName(texture);
        sprite->setTextureName(name);
    }
    ImGui::Text("Current texture: ");
    ImGui::SameLine();
    ImGui::PushID(id++);
    if (ImGui::Button(name.data())) {
        ImGui::OpenPopup("Textures buffer");
    }
    ImGui::SameLine();
    if (ImGui::ImageButton(*texture, sf::Vector2f(64, 64))) _imgWindow = true;
    if (_imgWindow) imageViewer(texture);
    if (ImGui::BeginPopup("Textures buffer")) {
        for (auto& it : R_GET_RESSOURCES(sf::Texture)) {
            auto& s = it.first;

            if (ImGui::Selectable(s.data())) {
                auto size = it.second.getSize();
                sprite->setTextureName(s);
                sprite->setTexture(it.second, true);
                ImGui::CloseCurrentPopup();
                break;
            }
        }
        ImGui::EndPopup();
    }
    ImGui::Text("Color: ");
    ImGui::SameLine();
    ImGui::InputScalarN("##SpriteColor", ImGuiDataType_U8, colorArr, 4);
    color = { colorArr[0], colorArr[1], colorArr[2], colorArr[3]};
    ImGui::SameLine();
    ImGui::Image(_colorSprite, sf::Vector2f(16, 16), color);
    sprite->setColor(color);
    ImGui::PopID();
}

void EngineHud::ComponentTreeNodeFactory::buildIdTreeNode(IComponent *c)
{
    auto id = dynamic_cast<Id *>(c);
    std::string idStr = "Id: " + std::to_string(id->get_id());

    ImGui::Text(idStr.data());
}

void EngineHud::ComponentTreeNodeFactory::buildAnimatorTreeNode(IComponent *c)
{
    auto animator = dynamic_cast<Animator *>(c);

    if (ImGui::SmallButton("+")) animator->newAnimation(0,
                                                        {0, 0, 0, 0},
                                                        0.0f,
                                                        "new animation");
    if (animator->empty()) {
        ImGui::Text("No animation pushed");
        return;
    }
    auto currentAnimation = animator->currentAnimation();
    auto currentAnimationName = currentAnimation.name();

    ImGui::Text("Current played animation: ");
    ImGui::SameLine();
    ImGui::Text(currentAnimationName.data());
    // handles animations
    if (ImGui::TreeNode("Animations")) {
        // Parse all Animations
        auto& animations = animator->getAnimations();

        int labelIndex = 0;
        for (auto& animation : animations) {
            ImGui::Separator();

            auto animationName = animation.name();
            auto offsetRect =  animation.getOffsetRect();
            float animationRect[4] = { offsetRect[0], offsetRect[1], offsetRect[2], offsetRect[3] };
            std::size_t frameNumber = animation.getFramesNumber();
            auto speed = animation.getSpeed();

            std::string labelIndexStr = std::to_string(labelIndex);
            ImGui::Text("Animation name: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100);
            ImGui::InputText(("##animationName" + labelIndexStr).data(), animationName.data(), 4096);
            ImGui::SetNextItemWidth(300);
            ImGui::Text("Rect");
            ImGui::SameLine();
            ImGui::InputFloat4(("##Rect" + labelIndexStr).data(), animationRect);
            ImGui::SetNextItemWidth(100);
            ImGui::Text("Frame number");
            ImGui::SameLine();
            ImGui::InputScalar(("##Frame number" + labelIndexStr).data(), ImGuiDataType_U64, &frameNumber);
            ImGui::Text("Animation speed: ");
            ImGui::InputFloat(("##AnimSpeed" + labelIndexStr).data(), &speed);
            ImGui::Separator();

            // set data
            labelIndex++;
            animation.setSpeed(speed);
            animation.setName(animationName);
            animation.setFramesNumber(frameNumber);
            animation.setOffsetRect( { animationRect[0], animationRect[1], animationRect[2], animationRect[3] });
        }
        ImGui::TreePop();
    }
}

void EngineHud::ComponentTreeNodeFactory::buildGravityTreeNode(IComponent *c)
{
    auto gravity = dynamic_cast<Gravity *>(c);

    ImGui::Text("Value: ");
    ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("##gravityforce", &gravity->force);
}

void EngineHud::ComponentTreeNodeFactory::buildLightTreeNode(IComponent *c)
{
    auto light = dynamic_cast<Light *>(c);
    auto sprite = light->getSprite();
    auto intensity = light->getIntensity();

    // create the sprite if it does not exist
    if (!sprite) {
        auto baseTextures = R_GET_RESSOURCES(sf::Texture);
        auto *s = new Sprite(baseTextures.begin()->second);
        s->setScale(2, 2);
        s->setTextureName(baseTextures.begin()->first);
        light->setSprite(s);
        sprite = s;
    }
    auto scale = sprite->getScale();
    if (ImGui::TreeNode(sprite->getTextureName().data())) {
        EngineHud::ComponentTreeNodeFactory::buildSpriteTreeNode(sprite);
        ImGui::TreePop();

        ImGui::Text("Light scale: x");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("##lightSizex", &scale.x);
        ImGui::SameLine();
        ImGui::Text("y: ");
        ImGui::SetNextItemWidth(100);
        ImGui::SameLine();
        ImGui::InputFloat("##lightSizey", &scale.y);
        sprite->setScale(scale.x, scale.y);
        light->setSprite(sprite);
    }
    ImGui::Text("Intensity: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("##Intensity", &intensity);
    light->setIntensity(intensity);
}

void EngineHud::canvasRadioButton(CanvasObject::CoordType& selectedOption, CanvasObject *obj)
{
    static std::map<std::string, CanvasObject::CoordType> typeMap = {
            { "Local", CanvasObject::CoordType::LOCAL },
            { "World", CanvasObject::CoordType::WORLD },
    };

    for (auto& type : typeMap) {
        auto name = type.first;
        auto index = type.second;

        bool selected = (selectedOption == index);
        if (ImGui::RadioButton(name.data(), selected)) {
            selectedOption = index;
            obj->coordType = index;
        }
        if (name.find("Local") != std::string::npos) ImGui::SameLine();
    }
}

void EngineHud::ComponentTreeNodeFactory::buildCanvasTreeNode(IComponent *c)
{
    auto canvas = dynamic_cast<Canvas *>(c);
    auto& buttons = canvas->getButtons();
    auto& texts = canvas->getTexts();
    auto& images = canvas->getImages();
    std::size_t id = 0;

    if (ImGui::TreeNode("Buttons")) {
        for (auto& it : buttons) {
            auto button = it.first;
            auto& offset = it.second;
            auto sprite = button->getSprite();
            std::string label = "##button" + std::to_string(id);
            CanvasObject::CoordType selectedOption = button->coordType;

            ImGui::Separator();
            ImGui::Text("position");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat((label + "positionX").data(), &offset.x);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat((label + "positionY").data(), &offset.y);
            ImGui::Text("Coordinate type");
            ImGui::SameLine();

            canvasRadioButton(selectedOption, button);
            EngineHud::ComponentTreeNodeFactory::buildSpriteTreeNode(sprite);
            auto pos = ImGui::GetCursorPosX();
            ImGui::SetCursorPosX(pos + 150);
            if (ImGui::Button("x", ImVec2(20, 20))) {
                canvas->removeObject<Button>(button);
                buttons.erase(button);
                continue;
            }
            id++;
        }
        ImGui::Separator();
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Texts")) {
        for (auto &it : texts) {
            auto& text = it.first;
            auto& offset = it.second;
            std::string content = text->getString();
            std::string label = "##texts" + std::to_string(id);
            auto size = text->getCharacterSize();
            CanvasObject::CoordType selectedOption = text->coordType;

            ImGui::Separator();
            ImGui::Text("Position");
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat((label + "x").data(), &offset.x);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat((label + "y").data(), &offset.y);
            ImGui::SetNextItemWidth(300);
            ImGui::InputText((label + "Content").data(), content.data(), 4096);
            ImGui::SetNextItemWidth(100);
            ImGui::InputInt((label + "Character size").data(), (int *)(&size));
            canvasRadioButton(selectedOption, text);

            if (ImGui::Button("x", ImVec2(20, 20))) {
                canvas->removeObject<Text>(text);
                texts.erase(text);
                continue;
            }
            text->setString(content.data());
            text->setCharacterSize(size);
            id++;
        }
        ImGui::Separator();
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Images")) {
        for (auto& it : images) {
            std::string label = "##toto" + std::to_string(id);
            auto& image = it.first;
            auto& offset = it.second;
            auto sprite = image->getImage();
            CanvasObject::CoordType selectedOption = image->coordType;

            ImGui::Separator();
            ImGui::Text("Position");
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat((label + "x").data(), &offset.x);
            ImGui::SameLine();
            ImGui::InputFloat((label + "y").data(), &offset.y);
            canvasRadioButton(selectedOption, image);
            EngineHud::ComponentTreeNodeFactory::buildSpriteTreeNode(sprite);
            if (ImGui::Button("x", ImVec2(20, 20))) {
                canvas->removeObject<Image>(image);
                images.erase(image);
                continue;
            }
            id++;
        }
        ImGui::Separator();
        ImGui::TreePop();
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

void EngineHud::saveResource(nlohmann::json& json, const std::string& entityPath)
{
    json["resources"].clear();
    auto textures = R_GET_RESSOURCES(sf::Texture);
    auto sounds = R_GET_RESSOURCES(sf::SoundBuffer);
    auto scripts = R_GET_SCRIPTS();
    auto musics = R_GET_MUSICS();

    // handle texture saving
    for (auto& it : textures) {
        auto& name = it.first;
        auto path = R_PATH_FROM_NAME(name);
        auto type = "Texture";
        auto textureJson = nlohmann::json();

        textureJson["path"] = path;
        textureJson["name"] = name;
        textureJson["type"] = type;
        json["resources"].push_back(textureJson);
    }

    // handle sounds saving
    for (auto& it : sounds) {
        auto& name = it.first;
        auto path = R_PATH_FROM_NAME(name);
        auto type = "Sound";
        auto soundJson = nlohmann::json();

        soundJson["path"] = path;
        soundJson["name"] = name;
        soundJson["type"] = type;
        json["resources"].push_back(soundJson);
    }

    // handle scripts saving
    for (auto& it : scripts) {
        auto& path = it.first;
        auto type = "Script";
        auto scriptJson = nlohmann::json();

        scriptJson["path"] = path;
        scriptJson["type"] = type;
        json["resources"].push_back(scriptJson);
    }

    // handle musics saving
    for (auto& it : musics) {
        auto& name = it.first;
        auto path = R_PATH_FROM_NAME(name);
        auto type = "Music";
        auto musicJson = nlohmann::json();

        musicJson["path"] = path;
        musicJson["name"] = name;
        musicJson["type"] = type;
        json["resources"].push_back(musicJson);
    }

    // handle entity path
    nlohmann::json entityPathJson = nlohmann::json();
    entityPathJson["type"] = "Entities";
    entityPathJson["path"] = entityPath;
    json["resources"].push_back(entityPathJson);

    auto content = json.dump(4);
    Utils::writeFile(_currentSceneFilepath + std::to_string(1), content);
}

void EngineHud::saveEntity(nlohmann::json &json)
{
    json["entities"].clear();
    for (auto& e : _toSave) {
        auto& name = e->getComponent<Tag>()->value();

        json["entities"].push_back(name);
    }
    auto c = json["entities"].dump(4);
    std::cout << c << std::endl;
}

void EngineHud::saveScene()
{
    if (Input::isKeyPressed("LControl") &&
        Input::isKeyDown("S")) {
        try {
            saveEntity(_currentSceneContent);
            std::unordered_map<std::string, Entity *> toSaves = getEntitiesNameToSave(
                                                                        _currentSceneContent["entities"]);
            std::string entitiesPath;
            for (auto& r : _currentSceneContent["resources"]) {
                if (r["type"].get<std::string>().find("Entities") != std::string::npos)
                    entitiesPath = r["path"];
            }
            saveResource(_currentSceneContent, entitiesPath);
            auto entitiesContentJson = Utils::readfileToJson(entitiesPath);

            for (auto &it : toSaves) {
                auto& name = it.first;
                auto *e = it.second;
                bool find = false;

                for (auto& json : entitiesContentJson["entities"]) {
                    auto entityName = json["name"];
                    if (entityName.get<std::string>().find(name) != std::string::npos) {
                        componentSerializer(json, e);
                        find = true;
                        break;
                    }
                }

                // if entity not found create it
                if (!find) {
                    auto newEntity = nlohmann::json();
                    newEntity["name"] = name;
                    newEntity["components"] = {};

                    componentSerializer(newEntity, e);
                    entitiesContentJson["entities"].push_back(newEntity);
                }
            }
            std::string content = entitiesContentJson.dump(4);
            Utils::writeFile(entitiesPath + std::to_string(1), content);
        } catch (std::exception& msg) {
            std::cerr << msg.what() << std::endl;
        }
    }
}

void EngineHud::createEntity()
{
    static std::string entityName;
    static bool show = false;

    ImGui::SetCursorPosX(((_height * GUI_ENTITIES_HEIGHT_SIZE_RATIO) / 2) / 2);
    if (ImGui::Button("New entity")) {
        show = true;
    }

    if (!show) return;
    ImGui::InputText("name", entityName.data(), 256);

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
        if (std::string(entityName.data()).empty()) {
            show = false;
            return;
        }
        auto *e = new Entity();

        e->addComponent<Tag>(entityName.data());
        e->addComponent<Transform2D>(0, 0);
        _toSave.push_back(e);
        System::forceUpdate(e);
        show = false;
        entityName.clear();
        std::cout << "[GUI] Creating a new entity " << entityName.data() << std::endl;
    }
}

void EngineHud::destroyEntity(Entity *e, const std::string& name)
{
    auto popUpName = name + " Entity actions";

    if (ImGui::IsItemHovered()) {
        if (ImGui::IsMouseDown(1)) {
            ImGui::OpenPopup(popUpName.data());
        }
    }
    if (ImGui::BeginPopup(popUpName.data())) {
        if (ImGui::Selectable("Destroy")) {
            _toSave.erase(std::remove(_toSave.begin(), _toSave.end(), e), _toSave.end());
            System::RemoveEntity(e);
            _selected = nullptr;
            std::cout << "[GUI] Destroy an entity " << name << std::endl;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void EngineHud::destroyTilemap(TileMap *tilemap, const std::string& name)
{
    auto popUpName = name + " Layer actions";

    if (ImGui::IsItemHovered()) {
        if (ImGui::IsMouseDown(1)) {
            ImGui::OpenPopup(popUpName.data());
        }
    }
    if (ImGui::BeginPopup(popUpName.data())) {
        if (ImGui::Selectable("Destroy")) {
            tilemap->destroy();
            std::cout << "[GUI] Destroy a layer " << name << std::endl;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
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
        destroyEntity(e, name);
        index++;
    }
    /* Handle tiles */
    layersEntity(index, tileMap);

    // Add entity Button
    createEntity();
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
                destroyEntity(e, name);
                index++;
            }
            ImGui::TreePop();
        }
        destroyTilemap(layer, layerName);
        layerIndex++;
    }
}

void EngineHud::createComponent()
{
    ImGui::SetCursorPosX(((_height * GUI_ENTITIES_HEIGHT_SIZE_RATIO) / 2) / 2);
    std::array<std::string, 14> componentList = {
            "Transform2D",
            "BoxCollider",
            "Sprite",
            "Velocity",
            "Animator",
            "View",
            "Tag",
            "Layer",
            "Sound",
            "Music",
            "Script",
            "Light",
            "Gravity",
            "Canvas"
    };

    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("components creation");
    }

    if (ImGui::BeginPopup("components creation")) {
        for (auto& component : componentList) {
            if (ImGui::Selectable(component.data())) {
                ComponentCreationFactory::create(component, _selected);
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }
}

void EngineHud::destroyComponent(IComponent *c, const std::string& name)
{
    auto popUpName = name + " Components actions";

    if (name.find("Transform2D") != std::string::npos ||
        name.find("Layer") != std::string::npos ||
        name.find("Id") != std::string::npos) return;
    if (ImGui::IsItemHovered()) {
        if (ImGui::IsMouseDown(1)) {
            ImGui::OpenPopup(popUpName.data());
        }
    }
    if (ImGui::BeginPopup(popUpName.data())) {
        if (ImGui::Selectable("Destroy")) {
            _selected->removeComponent(c);
            c->destroy();
            std::cout << "[GUI] Destroy an entity component " << name << std::endl;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
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
            destroyComponent(elem.second, updated);
            i++;
        }
        createComponent();
    }
    id = 0;
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

void EngineHud::scriptEditor(Script *script)
{
    if (_lastScript != script) {
        _lastScript = script;
        _scriptContent = R_GET_SCRIPT(script->getFile());
    }
    static bool open = true;
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
    ImGui::Begin("File Editor", &open);
    if (!open) {
        _scriptWindow = false;
        open = true;
        ImGui::End();
        return;
    }
    if (ImGui::Button("Save File")) {
        Utils::writeFile(script->getFile(), _scriptContent);
        script->reload();
    }
    ImGui::Separator();
    ImGui::InputTextMultiline("##editor", _scriptContent.data(), 4096,
      ImVec2(-1.0f, -1.0f), ImGuiInputTextFlags_AllowTabInput);
    ImGui::End();
}

void EngineHud::baseResourceForm(const std::string& type, bool showName)
{
    std::string typeText = "Type: " + type;

    ImGui::Separator();
    ImGui::Text(typeText.data());
    ImGui::SetNextItemWidth(500);
    if (_inputPath.size() != 4096) _inputPath.reserve(4096);
    if (showName) {
        ImGui::Text("Name: ");
        ImGui::SameLine();
        ImGui::InputText("##inputName", _inputName.data(), 4096);
    }
    ImGui::Text("Path: ");
    ImGui::SameLine();
    ImGui::InputText("##inputPath", _inputPath.data(), 4096);
}

void EngineHud::resourceManager()
{
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Resource Manager");
    static std::string selected;
    std::string types[] = {
            "Sound",
            "Music",
            "Texture",
            "Tile",
            "Script"
    };

    if (ImGui::TreeNode("Textures")) {
        auto& resource = R_GET_RESSOURCES(sf::Texture);
        resourceManagerResourceTreeNodeContent<sf::Texture>(resource);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Music")) {
        auto& resource = R_GET_MUSICS();
        resourceManagerResourceTreeNodeContent<sf::Music *>(resource);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Sound")) {
        auto& resource = R_GET_RESSOURCES(sf::SoundBuffer);
        resourceManagerResourceTreeNodeContent<sf::SoundBuffer>(resource);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Script")) {
        auto& resource = R_GET_SCRIPTS();
        resourceManagerResourceTreeNodeContent<std::string>(resource);
        ImGui::TreePop();
    }

    if (ImGui::Button("ADD resource")) {
        ImGui::OpenPopup("Resource type");
    }

    if (ImGui::BeginPopup("Resource type")) {

        for (auto &it : types) {
            if (ImGui::Selectable(it.data())) {
                selected = it;
            }
        }
        ImGui::EndPopup();
    }

    if (!selected.empty()) {
        ResourceType& type = _typeMap[selected];

        switch (type) {
            case SoundR:
                baseResourceForm(selected, true);
                break;
            case MusicR:
                baseResourceForm(selected, true);
                break;
            case TextureR:
                baseResourceForm(selected, true);
                break;
            case ScriptR:
                baseResourceForm(selected, false);
                break;
            case TileR:
                baseResourceForm(selected, true);
                // tile info
                ImGui::Text("Tile Informations: ");
                ImGui::InputFloat4("##tileInfo", _tileInfo);
                break;
            default: break;
        }

        if (ImGui::SmallButton("+")) {
            switch (type) {
                case SoundR:
                    R_ADD_RESSOURCE(sf::SoundBuffer, _inputName.c_str(), _inputPath.c_str());
                    break;
                case MusicR:
                    R_ADD_MUSIC(_inputName.c_str(), _inputPath.c_str());
                    break;
                case TextureR:
                    R_ADD_RESSOURCE(sf::Texture, _inputName.c_str(), _inputPath.c_str());
                    break;
                case TileR:
                    R_ADD_TILE(_inputName.c_str(), _inputPath.c_str(), _tileInfo[0],_tileInfo[1], _tileInfo[2], _tileInfo[3]);
                    break;
                case ScriptR:
                    R_ADD_SCRIPT(_inputPath.c_str());
                default: break;
            }
            // reset all values
            _tileInfo[0] = 0.0f;
            _tileInfo[1] = 0.0f;
            _tileInfo[2] = 0.0f;
            _tileInfo[3] = 0.0f;
            _inputPath.clear();
            _inputName.clear();
            selected.clear();
        }

    }

    ImGui::End();
}

void EngineHud::imageViewer(const sf::Texture *texture)
{
    static bool open = true;

    ImGui::SetNextWindowSize(ImVec2(800, 800));
    ImGui::Begin("Image Viewer", &open);
    if (!open) {
        _imgWindow = false;
        open = true;
        ImGui::End();
        return;
    }
    ImGui::Image(*texture, sf::Vector2f(texture->getSize().x, texture->getSize().y));
    ImGui::End();
}

template <typename T>
void EngineHud::resourceManagerResourceTreeNodeContent(std::map<std::string, T> &resource)
{
    for (auto& it : resource) {
        auto& name = it.first;
        auto path = R_PATH_FROM_NAME(name);

        ImGui::Separator();
        ImGui::Text(name.data());
        ImGui::SameLine();
        ImGui::Text("path: ");
        ImGui::SameLine();
        ImGui::Text(path.data());
        ImGui::SameLine();
        if (ImGui::SmallButton("x")) {
            auto f = resource.find(it.first);
            resource.erase(f);
            return;
        }
        ImGui::Separator();
    }
}
