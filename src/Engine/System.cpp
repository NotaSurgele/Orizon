#include "Engine/System.hpp"
#include "Components/View.hpp"
#include "Core.hpp"
#include <limits>

void System::velocity_system(Entity *e)
{
    auto velocity = e->getComponent<Velocity<float>>();
    auto transform = e->getComponent<Transform2D>();

    if (!velocity || !transform)
        return;
    transform->position.x += velocity->getX() * Time::deltaTime;
    transform->position.y += velocity->getY() * Time::deltaTime;
}

void System::draw_system()
{
    for (auto it : _registry) {
        auto e = *(it.second);
        auto sprite = e->getComponent<Sprite>();
        auto transform = e->getComponent<Transform2D>();
        auto currentView = Window.getView();

        if (!sprite)
            return;
        if (!transform)
            transform = Transform2D::zero();
        sprite->setTransform(transform);
        if (currentView != nullptr) {
            sf::Vector2f fix_pos = currentView->getCenter() - (currentView->getSize() / 2.0f);
            sf::FloatRect bounds = sf::FloatRect(fix_pos, currentView->getSize());

            if (bounds.contains(transform->position))
                DRAW(sprite);
        } else
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

        if (entity2 == entity) return;
        if (!collider2) return;
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

        if (!collider) return;
        if (!transform) transform = Transform2D::zero(), destroy_t = true;
        if (!velocity) return;
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

void System::gravity_system(Entity *e)
{
    auto velocity = e->getComponent<Velocity<float>>();
    auto gravity = e->getComponent<Gravity>();
    auto collider = e->getComponent<BoxCollider>();

    if (!velocity || !gravity)
        return;
    if (collider) {
        (collider->getState() == BoxCollider::Collide::TRUE) ?
            velocity->setY(0) : velocity->setY(gravity->force);
        return;
    }
    velocity->setY(gravity->force);
}

void System::update_custom_component(Entity *e)
{
    auto map = e->getCustomComponents();

    for (auto &it2 : map) {
        auto &component = it2.second;

        component->update();
    }
}

void System::camera_system(Entity *e)
{
    auto view = e->getComponent<View>();
    auto transform = e->getComponent<Transform2D>();
    bool destroy = false;

    if (!view)
        return;
    if (!view->isFollowing())
        return;
    if (!transform) {
        destroy = true;
        transform = Transform2D::zero();
    }
    view->setCenter(transform->position);
    SET_VIEW(view);
    if (destroy)
        transform->destroy();
}

void System::quad_collision_system()
{
    _quad->collide();
    _quad->show();
}

void System::box_system(Entity *e)
{
    auto transform = e->getComponent<Transform2D>();
    auto velocity = e->getComponent<Velocity<float>>();
    auto box = e->getComponent<BoxCollider>();
    bool d_v = false;

    if (!box)
        return;
    if (!velocity) {
        velocity = Velocity<float>::zero(), 
        d_v = true;
    }
    float x = velocity->getX() > 0 ? 1 : velocity->getX() < 0 ? -1 : 0;
    float y = velocity->getY() > 0 ? 1 : velocity->getY() < 0 ? -1 : 0;
    box->setPosition(transform->position.x + x, transform->position.y + y);
    if (d_v)
        delete velocity;
}

void System::merge()
{
    draw_system();
    for (auto& it : _registry) {
        auto& e = *(it.second);

        camera_system(e);
        gravity_system(e);
        box_system(e);
        // collider_system();
        // quad_collision_system();
        velocity_system(e);
        update_custom_component(e);
    }
}

void System::systems()
{
    merge();
}
