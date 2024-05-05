#pragma once

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <functional>
#include "Core.hpp"
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
    Particle(const std::string& file);
    Particle();
    ~Particle() = default;

private:
    static std::unordered_map<std::string, std::function<void(nlohmann::json&)>> _behaviourMap;
    std::vector<Sprite *> _sprites;
    nlohmann::json _json;
};
