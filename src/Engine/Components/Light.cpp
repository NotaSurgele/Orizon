#include "Light.hpp"
#include "System.hpp"
#include <math.h>

Light::Light(Entity *e) : _e(e)
{
    for (double angle = 0; angle < 360; angle += 1) {
        RayCaster ray(_e->getComponent<Transform2D>()->position, sf::Vector2f(1, 0), 300);
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
    // FIXME: Not all light dissapear
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

                boxSprite->setColor(this->applyLightEffect(attenuation * 300));
                boxSprite->setLightApply(true);
            }
        }
        angle = angleCounter.fetch_add(1, std::memory_order_relaxed);
    }
}

void Light::emit(const std::vector<Entity *>& entities)
{
    std::vector<std::thread> threads;
    std::atomic<int> angleCounter(0);

    for (int i = 0; i < 50; ++i) {
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