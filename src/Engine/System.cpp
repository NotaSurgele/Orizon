#include "Engine/System.hpp"
#include "Components/View.hpp"
#include "Core.hpp"
#include <limits>

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
        if (!transform)
            transform = Transform2D::zero();
        sprite->setTransform(transform);
        DRAW(sprite);
    }
}

void System::collider_system_check_entity(Entity *entity, BoxCollider *collider, Velocity<float> *velocity)
{
    for (auto &it2 : _registry) {
        bool destroy_v = false;
        bool destroy_t = false;
        auto entity2 = (*it2.second.get());
        auto collider2 = entity2->getComponent<BoxCollider>();
        auto velocity2 = entity2->getComponent<Velocity<float>>();
        auto transform2 = entity2->getComponent<Transform2D>();

        if (entity2 == entity) continue;
        if (!collider2) continue;
        if (!transform2) transform2 = Transform2D::zero(), destroy_t = true;
        if (!velocity2)  velocity2 = Velocity<float>::zero(), destroy_v = true;
        float valx = velocity2->getX() > 0 ? 1 : velocity2->getX() < 0 ? -1 : 0;
        float valy = velocity2->getY() > 0 ? 1 : velocity2->getY() < 0 ? -1 : 0;
        sf::Vector2<float> predicted_pos2 = sf::Vector2<float>(
                                    transform2->position.x + valx,
                                    transform2->position.y + valy);
        collider2->setPosition(predicted_pos2);
        if (collider->overlap(collider2)) {
            collider->setState(BoxCollider::Collide::TRUE);
            velocity->setX(0.0f);
            velocity->setY(0.0f);
            velocity2->setX(0.0f);
            velocity2->setY(0.0);
            return;
        } else
            collider->setState(BoxCollider::Collide::FALSE);
        if (destroy_v) velocity2->destroy();
        if (destroy_t) transform2->destroy();
    }
}

void System::collider_system()
{
    for (auto &it : _registry) {
        auto entity = (*it.second.get());
        auto collider = entity->getComponent<BoxCollider>();
        bool destroy_t = false;
        auto velocity = entity->getComponent<Velocity<float>>();
        auto transform = entity->getComponent<Transform2D>();

        if (!collider) continue;
        if (!transform) transform = Transform2D::zero(), destroy_t = true;
        if (!velocity) continue;
        float valx = velocity->getX() > 0 ? 1 : velocity->getX() < 0 ? -1 : 0;
        float valy = velocity->getY() > 0 ? 1 : velocity->getY() < 0 ? -1 : 0;
        sf::Vector2<float> predicted_pos = sf::Vector2<float>(transform->position.x
                                           + valx,
                                           transform->position.y +
                                           valy);
        collider->setPosition(predicted_pos);
        collider_system_check_entity(entity, collider, velocity);
        if (destroy_t) transform->destroy();
    }
}

void System::gravity_system()
{
    for (auto& it : _registry) {
        auto& entity = (*it.second.get());
        auto velocity = entity->getComponent<Velocity<float>>();
        auto gravity = entity->getComponent<Gravity>();
        auto collider = entity->getComponent<BoxCollider>();

        if (!velocity || !gravity)
            continue;
        if (collider) {
            (collider->getState() == BoxCollider::Collide::TRUE) ?
                velocity->setY(0) : velocity->setY(gravity->force);
            return;
        }
        velocity->setY(gravity->force);
    }
}

void System::update_custom_component()
{
    for (auto &it : _registry) {
        auto map = (*it.second.get())->getCustomComponents();

        for (auto &it2 : map) {
            auto &component = it2.second;

            component->update();
        }
    }
}

void System::camera_system()
{
    for (auto& it : _registry) {
        auto view = (*it.second)->getComponent<View>();
        auto transform = (*it.second)->getComponent<Transform2D>();
        bool destroy = false;

        if (!view)
            continue;
        if (!view->isFollowing())
            continue;
        if (!transform) {
            destroy = true;
            transform = Transform2D::zero();
        }
        view->setCenter(transform->position);
        SET_VIEW(view);
        if (destroy)
            transform->destroy();
    }
}

void System::systems()
{
    camera_system();
    gravity_system();
    collider_system();
    velocity_system();
    draw_system();
    update_custom_component();
}
