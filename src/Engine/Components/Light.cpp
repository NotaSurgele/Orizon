#include "Light.hpp"
#include <math.h>

Light::Light(Entity *e) : _e(e)
{
    for (double angle = 0; angle < 360; angle += 1) {
        RayTracer ray(_e->getComponent<Transform2D>()->position, sf::Vector2f(1, 0), 300);
        ray.rotate(angle);
        _rayCaster.push_back(ray);
    }
    auto color = sf::Color::White;
    int newRed = static_cast<int>(color.r * _intensity);
    int newGreen = static_cast<int>(color.g * _intensity);
    int newBlue = static_cast<int>(color.b * _intensity);
    Light::darkColor = sf::Color(newRed, newGreen, newBlue, color.a);
    System::lightSources += 1;
}

sf::Color Light::applyLightEffect(const float& attenuation)
{
    int newRed = static_cast<int>(Light::darkColor.r * attenuation);
    int newGreen = static_cast<int>(Light::darkColor.g * attenuation);
    int newBlue = static_cast<int>(Light::darkColor.b * attenuation);

    // Ensure the new values stay within the valid range
    if (newRed > 255) newRed = 255;
    if (newGreen > 255) newGreen = 255;
    if (newBlue > 255) newBlue = 255;
    if (newRed < Light::darkColor.r) newRed = Light::darkColor.r;
    if (newGreen < Light::darkColor.g) newGreen = Light::darkColor.g;
    if (newBlue < Light::darkColor.b) newBlue = Light::darkColor.b;
    return sf::Color(newRed, newGreen, newBlue, Light::darkColor.a);
}

void Light::emit(const std::vector<Entity *>& entities)
{
    double angle = 0.0f;

    for (auto ray : _rayCaster) {
        ray.setPosition(_e->getComponent<Transform2D>()->position);
        // ray.setDirection(sf::Vector2f(1, 0));
        ray.rotate(angle);
        for (auto e : entities) {
            auto box = e->getComponent<BoxCollider>();
            auto boxSprite = e->getComponent<Sprite>();
            // box->setColor(sf::Color::White);
            // sf::Color emission = sf
            if (ray.hit(box)) {
                auto point = ray.getCollisionPoint();
                auto position = ray.getPosition();

                float deltaX = point.x - position.x;
                float deltaY = point.y - position.y;
                float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
                float attenuation = (1.0f / (1.0f + 0.1f * distance + 0.01f * distance * distance)) * _intensity;
                boxSprite->setColor(this->applyLightEffect(attenuation * 300));
                // DRAW(box);
                // ray.show(1);
                break;
            }
        }
        angle++;
    }
}

void Light::destroy()
{
    System::lightSources -= 1;
    _rayCaster.clear();
}