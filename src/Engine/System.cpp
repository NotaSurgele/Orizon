#include "Engine/System.hpp"
#include "Components/View.hpp"
#include "Core.hpp"

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
    for (auto& it : _registry) {
        auto& e = (*it);
        if (!isInView(e))
            continue;
        auto sprite = e->getComponent<Sprite>();
        auto transform = e->getComponent<Transform2D>();
        auto currentView = Window.getView();

        if (!sprite)
            return;
        if (!transform)
            transform = Transform2D::zero();
        sprite->setTransform(transform);
        camera_system(e);
        DRAW(sprite);
        gravity_system(e);
        box_system(e);
        velocity_system(e);
        update_custom_component(e);
        _quad->insert(e);
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
    _quad->collide(*this);
    _quad->destroy();
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

void System::sort()
{
    std::sort(_registry.begin(), _registry.end(),
        [](const SharedEntity& se1, const SharedEntity& se2) {
            auto e1 = *se1;
            auto e2 = *se2;
            auto l1 = e1->getComponent<Layer>();
            auto l2 = e2->getComponent<Layer>();

            return l1->value() < l2->value();
        }
    );
}

void System::merge()
{
    // std::cout << _registry.size() << std::endl;
    draw_system();
    // collider_system();
    quad_collision_system();
}

bool System::isInView(Entity *e)
{
    auto transform = e->getComponent<Transform2D>();
    auto currentView = Window.getView();

    if (!transform)
        transform = Transform2D::zero();
    if (currentView != nullptr) {
        sf::Vector2f size = currentView->getSize();
        sf::Vector2f fix_pos = currentView->getCenter() - (currentView->getSize() / 2.0f);
        sf::FloatRect bounds = sf::FloatRect(fix_pos, currentView->getSize());

        return bounds.contains(transform->position);
    }
    return true;
}

void System::systems()
{
    merge();
}
