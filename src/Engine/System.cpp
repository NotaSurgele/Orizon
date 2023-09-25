#include "Engine/System.hpp"
#include "Components/View.hpp"
#include "Components/Light.hpp"
#include "Core.hpp"
#include "RayCaster.hpp"

void System::addEntity(Entity *entity)
{
    entity->addComponent<Id>(_id++);
    entity->addComponent<Layer>(0);
    Light::set = false;
    if (_registry_size >= 2) {
        auto it = _registry.begin();

        for (auto e : _registry) {
            auto v = e->getComponent<Layer>()->value();
            auto v2 = entity->getComponent<Layer>()->value();

            if (v > v2) {
                _registry.insert(it, entity);
                _registry_size++;
                return;
            }
            it++;
        }
    }
    _registry.push_back(entity);
    _registry_size++;
}

void System::velocity_system(Entity *e)
{
    auto velocity = e->getComponent<Velocity<float>>();
    auto transform = e->getComponent<Transform2D>();
    auto box = e->getComponent<BoxCollider>();

    if (!velocity || !transform)
        return;
    if (box != nullptr) {
        if (box->collide) {
            for (auto side : box->getSides()) {
                switch (side) {
                    case BoxCollider::Side::DOWN:
                        velocity->setY(0.0f);
                        break;
                    case BoxCollider::Side::TOP:
                        velocity->setY(0.0f);
                        break;
                    case BoxCollider::Side::LEFT:
                        velocity->setX(0.0f);
                        break;
                    case BoxCollider::Side::RIGHT:
                        velocity->setX(0.0f);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    transform->position.x += velocity->getX() * Time::deltaTime;
    transform->position.y += velocity->getY() * Time::deltaTime;
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

void System::sprite_system(Entity *e, std::vector<IComponent *> componentCache)
{
    auto sprite = e->getComponent<Sprite>();
    auto transform = e->getComponent<Transform2D>();

    if (!transform) {
        transform = Transform2D::zero();
        componentCache.push_back(transform);
    }
    if (!sprite)
        return;
    sprite->setTransform(transform);
    sprite->setLightApply(false);
    DRAW(sprite);
}

void System::systems()
{
    std::vector<IComponent *> componentCache;

    for (auto& e : _registry) {
        camera_system(e);
        auto sprite = e->getComponent<Sprite>();

        if (System::lightSources > 0 && !Light::set && sprite) {
            sprite->setColor(Light::darkColor);
        }
        if (!isInView(e))
            continue;
        _inView.push_back(e);
        // Test
        sprite_system(e, componentCache);
        light_system(e);
        gravity_system(e);
        BoxSystem(e);
        collider_system(e);
        velocity_system(e);
        update_custom_component(e);
    }
    for (auto &it : componentCache) {
        delete it;
        it = nullptr;
    }
    componentCache.clear();
    Light::set = true;
}

void System::light_system(Entity *e)
{
    auto light = e->getComponent<Light>();
    auto sprite = e->getComponent<Sprite>();

    if (sprite && !sprite->isLightApply()) {
        auto color = Light::darkColor;
        auto spriteIntensity = sprite->getShadowIntensity();

        if (spriteIntensity != .4f) {
            color = Light::loadColorFromIntensity(spriteIntensity);
            sprite->setColor(color);
        } else
            sprite->setColor(color);
    }
    if (!light)
        return;
    if (!isInView(e))
        return;
    if (_layers.size() > 0) {
        for (auto layer : _layers) {
            if (!layer->contain(e))
                continue;
            if (!light->isSpriteLoaded()) {
                std::vector<Entity *> entities = layer->checkEdges<Transform2D>(e, light->getEmission() / layer->tileWidth);

                light->emit(entities);
                continue;
            }
            light->emit();
        }
        return;
    }
    if (!light->isSpriteLoaded()) {
        light->emit(_registry);
        return;
    }
    light->emit();
}

void System::gravity_system(Entity *e)
{
    auto velocity = e->getComponent<Velocity<float>>();
    auto gravity = e->getComponent<Gravity>();
    auto collider = e->getComponent<BoxCollider>();

    if (!velocity || !gravity)
        return;
    if (collider->collide) {
        if (collider->hasSide(BoxCollider::Side::DOWN) ||
            collider->hasSide(BoxCollider::Side::DOWNLEFT) ||
            collider->hasSide(BoxCollider::Side::DOWNRIGHT)) {
            velocity->setY(0.0f);
            return;
        }
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

void System::collider_system(Entity *e)
{
    auto box = e->getComponent<BoxCollider>();
    int range = 0;

    if (box == nullptr)
        return;
    if (box->getType() == BoxCollider::Type::STATIC)
        return;
    range = box->getRange();
    if (range == 0)
        return;
    box->setColor(sf::Color::Green);
    box->sides.clear();
    box->side = BoxCollider::Side::NONE;
    box->collide = BoxCollider::Collide::FALSE;
    for (TileMap *layer : _layers) {
        float x = box->getPosition().x;
        float y = box->getPosition().y;

        if (!layer->contain(x, y))
            continue;
        std::vector<Entity *> arr = layer->checkAround<BoxCollider>(e, range);
        for (auto *entity : arr) {
            auto collider = entity->getComponent<BoxCollider>();

            box->collide = (collider->overlap(box)) ? BoxCollider::Collide::TRUE : BoxCollider::Collide::FALSE;

            // Resolve collision
            if (box->collide) {
                auto pos1 = box->getPosition();
                auto pos2 = collider->getPosition();
                auto size1 = box->getSize();
                auto size2 = collider->getSize();

                auto fixedSize1X = size1.x / 2;
                auto fixedSize1Y = size1.y / 2;
                auto fixedSize2X = size2.x / 2;
                auto fixedSize2Y = size2.y / 2;

                auto fixedPos1X = pos1.x + fixedSize1X;
                auto fixedPos1Y = pos1.y + fixedSize1Y;
                auto fixedPos2X = pos2.x + fixedSize2X;
                auto fixedPos2Y = pos2.y + fixedSize2Y;

                float dx = std::abs(fixedPos1X - fixedPos2X);
                float dy = std::abs(fixedPos1Y - fixedPos2Y);

                float overlapX = fixedPos1X + fixedPos2X - dx;
                float overlapY = fixedPos1Y + fixedPos2Y - dy;

                // Determine the direction of overlap
                if (overlapX < 0 || overlapY < 0) {
                    continue;
                }
                (fixedPos1X < fixedPos2X) ? box->registerSide(BoxCollider::Side::LEFT) : box->registerSide(BoxCollider::Side::RIGHT);
                (fixedPos1Y < fixedPos2Y) ? box->registerSide(BoxCollider::Side::DOWN) : box->registerSide(BoxCollider::Side::TOP);
            }
        }
        box->collide = (box->getSides().size() > 0) ? BoxCollider::Collide::TRUE : BoxCollider::Collide::FALSE;
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
    if (destroy) {
        transform->destroy();
        destroy = false;
    }
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
    float velX = velocity->getX();
    float velY = velocity->getY();
    float x = (velX > 0) ? 2 : (velX < 0) ? -2 : 0;
    float y = (velY > 0) ? 2 : (velY < 0) ? -2 : 0;

    box->setPosition(transform->position.x + x,
                    transform->position.y + y);
    if (d_v)
        delete velocity;
}

void System::merge()
{
    systems();
    //quad_collision_system();
}

bool System::isInView(Entity *e)
{
    auto transform = e->getComponent<Transform2D>();
    auto currentView = Window.getView();

    if (!transform)
        transform = Transform2D::zero();
    if (currentView != nullptr) {
        sf::Vector2f padding(0, 0);

        if (_layers.size() > 0) {
            padding.x = _layers[0]->tileWidth;
            padding.y = _layers[0]->tileHeight;
        }

        // FIX display when moving
        auto fixSize = currentView->getSize();
        sf::Vector2f fix_pos = currentView->getCenter() - (currentView->getSize() / 2.0f);

        fixSize.x += padding.x;
        fixSize.y += padding.y;
        fix_pos.x -= padding.x;
        fix_pos.y -= padding.y;
        sf::FloatRect bounds = sf::FloatRect(fix_pos, fixSize);
        return bounds.contains(transform->position);
    }
    return true;
}
