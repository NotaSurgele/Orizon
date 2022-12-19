#include "Engine/System.hpp"
#include "Components/BoxCollider.hpp"
#include "Core.hpp"

void System::velocity_system()
{
    for (auto &it : _registry) {
        Entity *e = *it.second.get();
        auto velocity = e->getComponent<Velocity<float>>();
        auto transform = e->getComponent<Transform2D>();

        if (!velocity || !transform)
            continue;
        transform->position.x += velocity->getX() * Time::deltaTime;
        transform->position.y += velocity->getY() * Time::deltaTime;
    }
}

void System::draw_system()
{
    for (auto &it : _registry) {
        auto entity = (*it.second.get());
        auto sprite = entity->getComponent<Sprite>();
        auto transform = entity->getComponent<Transform2D>();

        if (!sprite)
            continue;
        if (!entity->getComponent<Transform2D>())
            transform = Transform2D::zero();
        sprite->setTransform(transform);
        DRAW(sprite);
    }
}

void System::collider_system()
{
    for (auto &it : _registry) {
        auto entity = (*it.second.get());
        auto collider = entity->getComponent<BoxCollider>();
        auto velocity = entity->getComponent<Velocity<float>>();
        auto transform = entity->getComponent<Transform2D>();

        if (!collider) continue;
        if (!transform) transform = Transform2D::zero();
        if (!velocity) continue;
        float valx = velocity->getX() > 0 ? 1 : velocity->getX() < 0 ? -1 : 0;
        float valy = velocity->getY() > 0 ? 1 : velocity->getY() < 0 ? -1 : 0;
        //todo check if component velocity exist
        sf::Vector2<float> predicted_pos = sf::Vector2<float>(transform->position.x
                                           + valx,
                                           transform->position.y +
                                           valy);
        collider->setPosition(predicted_pos);
        for (auto &it2 : _registry) {
            auto entity2 = (*it2.second.get());
            auto collider2 = entity2->getComponent<BoxCollider>();
            auto velocity2 = entity2->getComponent<Velocity<float>>();
            auto transform2 = entity2->getComponent<Transform2D>();

            if (entity2 == entity) continue;
            if (!collider2) continue;
            if (!transform2) transform2 = Transform2D::zero();
            if (!velocity2)  velocity2 = Velocity<float>::zero();
            collider2->setPosition(transform2->position);
            if (collider->overlap(collider2)) {
                velocity->setX(0.0f);
                velocity->setY(0.0f);
                velocity2->setX(0.0f);
                velocity2->setY(0.0);
            }
        }
    }
}

void System::systems()
{
    collider_system();
    velocity_system();
    draw_system();
}
