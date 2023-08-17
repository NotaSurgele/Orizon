#pragma once

#include "Engine/Entity.hpp"
#include "RayCaster.hpp"
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
    void processLight(const std::vector<RayCaster>& rays, const std::vector<Entity*>& entities, std::atomic<int>& angleCounter);

private:
    std::vector<RayCaster> _rayCaster;
    sf::ConvexShape convex;
    Entity *_e = nullptr;
    float _intensity = .5f;
};