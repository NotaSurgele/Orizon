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
        sf::Image img = sf::Image();
        sf::Texture texture;

        img.create(16, 16, sf::Color::White);
        texture.loadFromImage(img);
        _colorSprite.setTexture(texture);
    }


    ~EngineHud() = default;

    void setTheme();
    void setCurrentSceneFilepath(const std::string& sceneFilepath);
    void currentSceneContent(const nlohmann::json& sceneContent);

    void entityWindow(const std::vector<Entity *>& _registry, const std::vector<TileMap *>& tileMap);
    void entityInformation();
    void consoleWindow();
    void resourceManager();

    static inline void registerSavedEntity(Entity *e)
    {
        _toSave.push_back(e);
    }

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

    void saveScene();
    void saveResource(nlohmann::json& json, const std::string& entityPath);
    void saveEntity(nlohmann::json& json);
    void createEntity();
    void destroyEntity(Entity *e, const std::string& name);

private:
    static inline std::string _msg;
    static inline bool _scriptWindow = false;

    static void scriptEditor(Script *component);

    template <typename T>
    static inline void writeConsole(const T& last)
    {
        std::string data = std::to_string(last);
        _msg += data;
        insert(_msg);
    }

    template <typename T = std::string>
    static inline void writeConsole(const std::string& last)
    {
        _msg += last;
        insert(_msg);
    }

    template <typename T>
    void resourceManagerResourceTreeNodeContent(std::map<std::string, T>& resource);

    std::unordered_map<std::string, Entity *> getEntitiesNameToSave(const nlohmann::json& entitiesJson);
    void componentSerializer(nlohmann::json& entityJson, Entity *e);
    void baseResourceForm(const std::string& type, bool showName=false);

    enum ResourceType {
        SoundR,
        MusicR,
        TextureR,
        TileR,
        ScriptR
    };

    class ComponentSerializerFactory {
    public:
        ComponentSerializerFactory() = default;
        ~ComponentSerializerFactory() = default;

        static inline nlohmann::json serialize(IComponent *component)
        {
            for (auto& it : _componentMap) {
                if (component->getSignature().find(it.first) != std::string::npos) {
                    return it.second(component);
                }
            }
            return {};
        }

    private:
        static nlohmann::json serializeTransform2D(IComponent *c);
        static nlohmann::json serializeBoxCollider(IComponent *c);
        static nlohmann::json serializeSprite(IComponent *c);
        static nlohmann::json serializeVelocity(IComponent *c);
        static nlohmann::json serializeAnimation(IComponent *c);
        static nlohmann::json serializeView(IComponent *c);
        static nlohmann::json serializeTag(IComponent *c);
        static nlohmann::json serializeLayer(IComponent *c);
        static nlohmann::json serializeSound(IComponent *c);
        static nlohmann::json serializeMusic(IComponent *c);
        static nlohmann::json serializeScript(IComponent *c);
        static nlohmann::json serializeLight(IComponent *c);
        static nlohmann::json serializeGravity(IComponent *c);


    private:
        static inline std::unordered_map<std::string, std::function<nlohmann::json(IComponent *)>> _componentMap = {
                { "Transform2D", serializeTransform2D },
                { "BoxCollider", serializeBoxCollider },
                { "Sprite", serializeSprite },
                { "Velocity", serializeVelocity },
                { "Animator", serializeAnimation },
                { "View", serializeView },
                { "Tag", serializeTag },
                { "Layer", serializeLayer },
                { "Sound", serializeSound },
                { "Music", serializeMusic },
                { "Script", serializeScript },
                { "Light", serializeLight },
                { "Gravity", serializeGravity }
        };

    };

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
        static void buildTransformTreeNode(IComponent *c);
        static void buildTagTreeNode(IComponent *c);
        static void buildViewTreeNode(IComponent *c);
        static void buildBoxColliderTreeNode(IComponent *c);
        static void buildVelocityTreeNode(IComponent *c);
        static void buildSoundTreeNode(IComponent *c);
        static void buildLayerTreeNode(IComponent *c);
        static void buildOrizonMusicTreeNode(IComponent *c);
        static void buildScriptTreeNode(IComponent *c);
        static void buildSpriteTreeNode(IComponent *c);
        static void buildIdTreeNode(IComponent *c);
        static void buildAnimatorTreeNode(IComponent *c);
        static void buildGravityTreeNode(IComponent *c);
        static void buildLightTreeNode(IComponent *c);

    private:
        static inline std::unordered_map<std::string, std::function<void(IComponent *)>> _map= {
            { "Transform2D", buildTransformTreeNode },
            { "Tag", buildTagTreeNode },
            { "View", buildViewTreeNode },
            { "BoxCollider", buildBoxColliderTreeNode },
            { "Velocity", buildVelocityTreeNode },
            { "Sound", buildSoundTreeNode },
            { "Layer", buildLayerTreeNode },
            { "OrizonMusic", buildOrizonMusicTreeNode },
            { "Script", buildScriptTreeNode },
            { "Sprite", buildSpriteTreeNode },
            { "Id", buildIdTreeNode },
            { "Animator", buildAnimatorTreeNode },
            { "Gravity", buildGravityTreeNode },
            { "Light", buildLightTreeNode  }
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

    // form information
    std::string _inputPath;
    std::string _inputName;
    float _tileInfo[4] = {};

    std::unordered_map<std::string, ResourceType> _typeMap = {
            { "Sound", ResourceType::SoundR },
            { "Music", ResourceType::MusicR },
            { "Texture", ResourceType::TextureR },
            { "Tile", ResourceType::TileR },
            { "Script", ResourceType::ScriptR }
    };

    // scripting
    static inline sf::Sprite _colorSprite;
    static inline std::string _scriptContent;
    static inline Script *_lastScript = nullptr;

    std::string _consoleInputText;

    static inline std::queue<std::string> _consoleMsg;
    static inline std::vector<Entity *> _toSave;
    static inline std::string _newVal = "new value";
};