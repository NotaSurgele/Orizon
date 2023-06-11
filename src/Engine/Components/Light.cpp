#include "Light.hpp"

Light::Light(Entity *e) : _e(e)
{
    for (double angle = 0; angle < 360; angle += 1) {
        RayTracer ray(sf::Vector2f(0, 0), sf::Vector2f(1, 0));
        ray.setPosition(_e->getComponent<Transform2D>()->position);
        ray.rotate(angle);
        _rayCaster.push_back(ray);
    }
}

void Light::emit(const std::vector<Entity *>& entities)
{
    double angle = 0.0f;

    for (auto ray : _rayCaster) {
        ray.setDirection(sf::Vector2f(1, 0));
        ray.setPosition(_e->getComponent<Transform2D>()->position);
        ray.rotate(angle);
        for (auto e : entities) {
            auto box = e->getComponent<BoxCollider>();

            if (ray.hit(box)) {
                ray.show(2);
                break;
            }
        }
        angle++;
    }
}

void Light::destroy()
{
    _rayCaster.clear();
}