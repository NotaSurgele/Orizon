#include "Engine/System.hpp"
#include "Components/View.hpp"
#include "Core.hpp"

void System::velocity_system(Entity *e)
{
    auto velocity = e->getComponent<Velocity<float>>();
    auto transform = e->getComponent<Transform2D>();

    if (!velocity || !transform)
        return;
    _quad->remove(e);
    transform->position.x += velocity->getX() * Time::deltaTime;
    transform->position.y += velocity->getY() * Time::deltaTime;
    _quad->insert(e);
}

void System::sort()
{
    std::sort(_registry.begin(), _registry.end(),
        [](Entity *e1, Entity *e2) {
            auto v1 = e1->getComponent<Layer>()->value();
            auto v2 = e2->getComponent<Layer>()->value();

            return v1 > v2;
        }
    );
}

void System::draw_system()
{
    for (auto& e : _registry) {
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
        BoxSystem(e);
        velocity_system(e);
        update_custom_component(e);
        _quad->insert(e);
    }
    // Collision checking
    for (auto &e : _registry) {
        auto box = e->getComponent<BoxCollider>();
        auto v = e->getComponent<Velocity<float>>();
        bool d_v = false;

        if (!v) d_v = true, v = Velocity<float>::zero();
        std::vector<Entity *> array = _quad->retrieve(e);
        std::cout << "ARRAY: " << array.size() << std::endl;
        for (auto &e2 : array) {
            if (e2 == e)
                continue;
            auto box2 = e->getComponent<BoxCollider>();
            auto v2 = e->getComponent<Velocity<float>>();
            bool d_v2 = false;

            if (!v2) d_v2 = true, v2 = Velocity<float>::zero();
            if (box->overlap(box2)) {
                v->reset();
                v2->reset();
                box->setState(BoxCollider::Collide::TRUE);
                return;
            } else
                box->setState(BoxCollider::Collide::FALSE);
            if (d_v2) delete v2;
        }
        if (d_v) delete v;
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
    // _quad->show();
    sf::Vector2f pos = Window.getView()->getCenter();
    sf::Vector2f size = Window.getView()->getSize();

    // std::cout << pos.x << " " << pos.y << " " << size.x << " " << size.y << std::endl;
}

void System::BoxSystem(Entity *e)
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
    box->setPosition(transform->position.x, transform->position.y);
    // DRAW(box);
    if (d_v)
        delete velocity;
}

void System::merge()
{
    draw_system();
    //quad_collision_system();
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
