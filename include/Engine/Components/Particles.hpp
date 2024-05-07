#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>

#include "Components/Sprite.hpp"
#include "IComponent.hpp"
#include "json.hpp"

class Particle;

class ParticlesEmitter : public IComponent {
public:
    explicit ParticlesEmitter(Entity *e) : _e(e) {};
    ~ParticlesEmitter() = default;

    void destroy() override final;

public:
    std::unordered_map<std::string, Particle> particles;

private:
    Entity *_e = nullptr;
};

class Particle {
public:
    explicit Particle(const std::string& file);
    Particle();
    ~Particle() = default;

    void load();
    void play(bool loop, const sf::Vector2f& entityPosition);
    bool hasFinished() const;
    void reset();
    void destroy();

public:
    std::size_t amount = 10;
    std::size_t amountMax = 10;
    std::size_t amountMin = 1;

    sf::Vector2f scale = { 1, 1 };
    sf::Vector2f velocity;
    sf::Vector2f emitterLength = { 1, 1 };

    float randomness = 0;
    float lifeTime = 1.0f;

    sf::Texture *texture = nullptr;

    sf::Color color = sf::Color::White;
    sf::FloatRect rect { 0, 0, 0, 0 };

    bool loop = true;

private:
    bool _hasFinished = false;
    std::unordered_map<std::string, std::function<void(nlohmann::json&)>> _behaviourMap;

    std::vector<Sprite *> _sprites;
    nlohmann::json _json;

    sf::Vector2f _offset;
};
