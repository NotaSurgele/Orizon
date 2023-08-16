#pragma once

#include "Engine/Entity.hpp"
#include "Raytracer.hpp"
#include <thread>
#include <atomic>
#include <mutex>

class Light : public IComponent {
public:
    Light(Entity *e);
    void destroy() override final;
    void emit(const std::vector<Entity *>& entities);
    sf::Color getDarkColor();

public:
    static inline sf::Color darkColor = sf::Color();
    static inline bool set = false;

private:
    sf::Color applyLightEffect(const float& attenuation);
    void processRays(const std::vector<RayTracer>& rays, const std::vector<Entity*>& entities, std::atomic<int>& angleCounter);
private:
    std::vector<RayTracer> _rayCaster;
    Entity *_e = nullptr;
    float _intensity = .5f;
};