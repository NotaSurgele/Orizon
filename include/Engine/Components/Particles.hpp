#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>
#include <queue>
#include <deque>

#include "Components/Sprite.hpp"
#include "IComponent.hpp"
#include "Timer.hpp"
#include "json.hpp"

struct ParticleData {
    ParticleData() = default;
    ~ParticleData() = default;

    bool operator!=(ParticleData const& other) const
    {
        return !(
            velocityData != other.velocityData ||
            spriteData != other.spriteData
        );
    }

    struct VelocityData {
        sf::Vector2f velocity { 0, 0 };
        sf::Vector2f minVelocity { 0, 0 };
        sf::Vector2f maxVelocity { 0, 0 };
        bool random = false;

        bool operator!=(const VelocityData& other) const
        {
            return (
                other.velocity.x != velocity.x ||
                other.velocity.y != velocity.y ||
                other.minVelocity.x != minVelocity.x ||
                other.minVelocity.y != minVelocity.y ||
                other.maxVelocity.x != maxVelocity.x ||
                other.maxVelocity.y != maxVelocity.y
            );
        }
    };

    struct SpriteData {
        Sprite *sprite = nullptr;
        sf::Color color = sf::Color::White;
        sf::Color currentColor = sf::Color::White;
        sf::Vector2f scale = { 1, 1 };
        bool blended = false;

        bool operator!=(const SpriteData& other) const
        {
            return (
                other.sprite != sprite ||
                other.currentColor.r != currentColor.r ||
                other.currentColor.g != currentColor.g ||
                other.currentColor.b != currentColor.b ||
                other.currentColor.a != currentColor.a ||
                other.color.r != color.r ||
                other.color.g != color.g ||
                other.color.b != color.b ||
                other.color.a != color.a ||
                other.scale.x != scale.x ||
                other.scale.y != scale.y
            );
        }
    };

    struct FadeInData {
        bool end = false;
        float speed = 1;
        sf::Color to = sf::Color::White;

        bool operator!=(const FadeInData& other)
        {
            return (
                end != other.end ||
                speed != other.speed ||
                to.r != other.to.r ||
                to.g != other.to.g ||
                to.b != other.to.b ||
                to.a != other.to.a
            );
        }
    };

    struct FadeOutData {
        bool end = false;
        float speed = 1;
        sf::Color to = sf::Color::White;
        Timer timer;

        bool operator!=(const FadeOutData& other)
        {
            return (
                end != other.end ||
                speed != other.speed ||
                to.r != other.to.r ||
                to.g != other.to.g ||
                to.b != other.to.b ||
                to.a != other.to.a
            );
        }
    };

    std::optional<VelocityData> velocityData;
    std::optional<FadeInData> fadeInData;
    std::optional<FadeOutData> fadeOutData;

    SpriteData spriteData = {};

    std::size_t id = 0;
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
    void play(const sf::Vector2f& entityPosition);
    bool hasFinished() const;
    void reset();
    void destroy();
    std::list<ParticleData>& getParticlesData();
    sf::RectangleShape& getEmitterShape();

private:
    void updateDelayTimer(Timer &timer);
    void resetSpriteData(ParticleData::SpriteData& spriteData, const sf::Vector2f& ePosition);
    void resetParticleData(ParticleData& pData) const;
    void resetFadeIn(std::optional<ParticleData::FadeInData>& fadeIn, ParticleData::SpriteData& spriteData);
    void resetFadeOut(std::optional<ParticleData::FadeOutData>& fadeOut, ParticleData::SpriteData& spriteData);

    void fadeInSystem(ParticleData::SpriteData& pData, std::optional<ParticleData::FadeInData>& fadeIn);
    void fadeOutSystem(ParticleData::SpriteData& pData, std::optional<ParticleData::FadeOutData>& fadeOut);
    void fadeSystem(ParticleData::SpriteData& spriteData, std::optional<ParticleData::FadeInData>& fadeIn,
                    std::optional<ParticleData::FadeOutData>& fadeOut);

    void velocitySystem(std::optional<ParticleData::VelocityData>& velocityData, sf::Vector2f& position);
    void spriteSystem(ParticleData::SpriteData& spriteData, Sprite *s, const sf::Vector2f& fixedPosition);

    bool killParticle(ParticleData& pData, std::queue<std::size_t>& removeQueue);

public:

    std::unordered_map<std::string, std::function<void(ParticleData&, nlohmann::json&)>> behaviourMap;
    nlohmann::json jsonData{};

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
    int _totalDeadParticle = 0;
    std::queue<std::size_t> _removeQueue;
    Timer delayTimer;
    bool _hasFinished = false;

    sf::RectangleShape _shape;

    // Handle particles
    std::list<ParticleData> _particles;
    std::deque<ParticleData> _deadParticle;


};

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
