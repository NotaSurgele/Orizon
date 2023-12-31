#include "Light.hpp"
#include "System.hpp"
#include "Core.hpp"
#include <math.h>

Light::Light(Entity *e, const float& emission, const float& intensity) : _e(e), _emission(emission), _intensity(intensity)
{
    for (double angle = 0; angle < 360; angle += 1) {
        RayCaster ray(_e->getComponent<Transform2D>()->position, sf::Vector2f(1, 0), emission);
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

Light::Light(Entity *e, const float& emission, Sprite *sprite, const float& intensity) : _sprite(sprite),
                                                                                        _emission(emission),
                                                                                        _e(e),
                                                                                        _intensity(intensity)
{
    _isSpriteLoaded = true;
    _transform = e->getComponent<Transform2D>();
    auto color = sf::Color::White;
    int newRed = static_cast<int>(color.r * _intensity);
    int newGreen = static_cast<int>(color.g * _intensity);
    int newBlue = static_cast<int>(color.b * _intensity);
    auto spriteColor = sf::Color::White;
    spriteColor.a = 40;
    _sprite->setColor(spriteColor);
    Light::darkColor = sf::Color(newRed, newGreen, newBlue, color.a);
    _sprite->setScale(2, 2);
    System::lightSources += 1;
}

sf::Color Light::applyLightEffect(const float& attenuation)
{
    int newRed = static_cast<int>(Light::darkColor.r * attenuation);
    int newGreen = static_cast<int>(Light::darkColor.g * attenuation);
    int newBlue = static_cast<int>(Light::darkColor.b * attenuation);

    if (newRed > 255) newRed = 255;
    if (newGreen > 255) newGreen = 255;
    if (newBlue > 255) newBlue = 255;
    if (newRed < Light::darkColor.r) newRed = Light::darkColor.r;
    if (newGreen < Light::darkColor.g) newGreen = Light::darkColor.g;
    if (newBlue < Light::darkColor.b) newBlue = Light::darkColor.b;
    return sf::Color(newRed, newGreen, newBlue, Light::darkColor.a);
}

void Light::reset(Sprite *sprite, RayCaster &ray)
{

}

void Light::processLight(const std::vector<RayCaster>& rays, const std::vector<Entity*>& entities, std::atomic<int>& angleCounter)
{
    std::lock_guard<std::mutex> lock(std::mutex);
    int angle = 0;

    while (angle < rays.size()) {
        RayCaster ray = rays[angle];
        ray.setPosition(_e->getComponent<Transform2D>()->position);
        ray.rotate(static_cast<double>(angle));

        for (auto e : entities) {
            auto boxSprite = e->getComponent<Sprite>();
            auto transform = e->getComponent<Transform2D>();

            if (ray.hit(transform)) {
                auto point = ray.getCollisionPoint();
                auto position = ray.getPosition();

                float deltaX = point.x - position.x;
                float deltaY = point.y - position.y;
                float squaredDistance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
                float attenuation = (1.0f / (1.0f + 0.1f * squaredDistance + 0.01f * squaredDistance * squaredDistance)) * _intensity;
                boxSprite->setColor(this->applyLightEffect(attenuation * ray.getLength()));
                boxSprite->setLightApply(true);
            }
        }
        angle = angleCounter.fetch_add(1, std::memory_order_relaxed);
    }
}

void Light::setEmission(const float &emission)
{
    _emission = emission;
}

void Light::setColor(const sf::Color &color)
{
    _sprite->setColor(color);
}

float Light::getEmission()
{
    return _emission;
}

float Light::getIntensity()
{
    return _intensity;
}

bool Light::isSpriteLoaded()
{
    return _isSpriteLoaded;
}

void Light::emit()
{
    auto texture = _sprite->getTexture();
    auto textureSize = texture->getSize();
    auto scale = _sprite->getScale();

    auto fixedPositionX = _transform->position.x - ((textureSize.x * scale.x) / 2);
    auto fixedPositionY = _transform->position.y  - ((textureSize.y * scale.y) / 2);

    _sprite->setPosition(fixedPositionX, fixedPositionY);

    /// CAREFUL
    //texture->setSmooth(true);
/*
    DRAW(_shape);
*/
    DRAW_BLEND(_sprite, sf::BlendAdd);
}

void Light::emit(const std::vector<Entity *>& entities)
{
    // Handle threads if no sprite is loaded
    std::vector<std::thread> threads;
    std::atomic<int> angleCounter(0);

    for (int i = 0; i < 20; ++i) {
        threads.emplace_back([this, &entities, &angleCounter]() {
            processLight(_rayCaster, entities, angleCounter);
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }

    // std::size_t counter = 0;
    // convex.setPointCount(points.size());

    // for (auto point : points) {
    //     convex.setPoint(counter, point);
    //     counter++;
    // }
    // convex.setFillColor(sf::Color::Red);
    // DRAW(convex);
}

void Light::destroy()
{
    System::lightSources -= 1;
    _rayCaster.clear();
}