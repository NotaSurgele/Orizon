#pragma once

#include "Engine/Entity.hpp"
#include "RayCaster.hpp"
#include <thread>
#include <atomic>
#include <mutex>

class Light : public IComponent {
public:
    Light(Entity *e, const float& emission, const float& intensity=.4f);
    Light(Entity *e, const float& emission, Sprite *sprite, const float& intensity=.4f);

    void destroy() override final;

    void emit(const std::vector<Entity *>& entities);
    void emit(void);
    void setEmission(const float& emission);

    void setColor(const sf::Color& color);
    float getEmission();
    float getIntensity();
    bool isSpriteLoaded();

    static inline sf::Color loadColorFromIntensity(const float& intensity, const sf::Color& color=sf::Color::White)
    {
        unsigned char newRed = static_cast<unsigned char>(color.r * intensity);
        unsigned char newGreen = static_cast<unsigned char>(color.g * intensity);
        unsigned char newBlue = static_cast<unsigned char>(color.b * intensity);
        return {newRed, newGreen, newBlue, color.a};
    }

public:
    static inline sf::Color darkColor = sf::Color();
    static inline bool set = false;

private:
    sf::Color applyLightEffect(const float& attenuation);
    void processLight(const std::vector<RayCaster>& rays, const std::vector<Entity*>& entities, std::atomic<int>& angleCounter);
    void reset(Sprite *sprite, RayCaster &ray);

private:
    float _emission = 1.0f;
    std::vector<RayCaster> _rayCaster;
    sf::ConvexShape convex;
    Entity *_e = nullptr;
    Transform2D *_transform = nullptr;
    float _intensity = .4f;
    Sprite *_sprite = nullptr;
    bool _isSpriteLoaded = false;
};