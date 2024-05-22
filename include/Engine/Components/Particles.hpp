#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>
#include <queue>

#include "Components/Sprite.hpp"
#include "IComponent.hpp"
#include "Timer.hpp"
#include "json.hpp"

class Particle;

class ParticlesEmitter : public IComponent {
public:
    explicit ParticlesEmitter(Entity *e) : _e(e) {};
    ~ParticlesEmitter() = default;

    void destroy() final;

    Entity *getEntity()
    {
        return _e;
    }

public:
    std::unordered_map<std::string, Particle> particles;

private:
    Entity *_e = nullptr;
};

struct ParticleData {
    ParticleData() = default;
    ~ParticleData() = default;

    struct VelocityData {
        sf::Vector2f velocity { 0, 0 };
        sf::Vector2f minVelocity { 0, 0 };
        sf::Vector2f maxVelocity { 0, 0 };
        bool random = false;
    };

    struct SpriteData {
        Sprite *sprite = nullptr;
        sf::Color color = sf::Color::White;
        sf::Color currentColor = sf::Color::White;
        sf::Vector2f scale = { 1, 1 };
    };

    struct FadeInData {
        bool end = false;
        float speed = 1;
        sf::Color to = sf::Color::White;
    };

    struct FadeOutData {
        bool end = false;
        float speed = 1;
        sf::Color to = sf::Color::White;
    };

    std::optional<VelocityData> velocityData;
    std::optional<FadeInData> fadeInData;
    std::optional<FadeOutData> fadeOutData;

    SpriteData spriteData = {};

    bool set = false;
    Timer lifeTimer;
    bool isDead = false;
};

class Particle {
public:
    explicit Particle(const std::string& file);
    Particle();
    ~Particle() = default;

    void reload();
    void load();
    void load(const std::size_t& amount);
    void initData(nlohmann::json& json);
    void play(bool loop, const sf::Vector2f& entityPosition);
    bool hasFinished() const;
    void reset();
    void destroy();
    std::list<ParticleData>& getParticlesData();


private:

    void resetSpriteData(ParticleData::SpriteData& spriteData, const sf::Vector2f& ePosition);
    void resetParticleData(ParticleData& pData) const;
    void resetFadeIn(std::optional<ParticleData::FadeInData>& fadeIn, ParticleData::SpriteData& spriteData);
    void resetFadeOut(std::optional<ParticleData::FadeOutData>& fadeOut, ParticleData::SpriteData& spriteData);

    void fadeInSystem(ParticleData::SpriteData& pData, std::optional<ParticleData::FadeInData>& fadeIn);
    void fadeOutSystem(ParticleData::SpriteData& pData, std::optional<ParticleData::FadeOutData>& fadeOut);
    void fadeSystem(ParticleData::SpriteData& spriteData, std::optional<ParticleData::FadeInData>& fadeIn,
                    std::optional<ParticleData::FadeOutData>& fadeOut);

    bool killParticle(ParticleData& pData, std::queue<std::size_t>& removeQueue, std::size_t& index, std::size_t& deadParticle);

public:
    std::string path;
    int seed = 0;

    int amount = 10;
    int amountMax = 10;
    int amountMin = 1;

    sf::Vector2f scale = { 1, 1 };
    sf::Vector2f emitterLength = { 1, 1 };

    float lifeTime = 1.0f;
    float delay = 0.0F;

    sf::Texture *texture = nullptr;

    sf::Color color = sf::Color::White;
    sf::FloatRect rect { 0, 0, 0, 0 };

    bool loop = true;

private:
    std::queue<std::size_t> _removeQueue;
    Timer delayTimer;
    bool _hasFinished = false;
    std::unordered_map<std::string, std::function<void(ParticleData&, nlohmann::json&)>> _behaviourMap;

    // Handle particles
    std::list<ParticleData> _particles;
    std::queue<ParticleData> _deadParticle;

    nlohmann::json _json;

    sf::Vector2f _offset = {0, 0};
};