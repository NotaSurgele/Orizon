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

void System::handle_velocity_colliding_sides(BoxCollider *box, Transform2D *transform, Velocity<float> *velocity)
{
    if (box->collide) {
        auto values = velocity->values();
        for (auto side : box->getSides()) {
            switch (side) {
                case BoxCollider::Side::DOWN:
                    if (values.y == 0.0f) {
                        transform->position.y -= 5;
                        break;
                    }
                    velocity->setY(0.0f);
                    break;
                case BoxCollider::Side::TOP:
                    if (values.y == 0.0f) {
                        transform->position.y += 5;
                        break;
                    }
                    velocity->setY(0.0f);
                    break;
                case BoxCollider::Side::LEFT:
                    if (values.x == 0.0f) {
                        transform->position.x -= 5;
                        break;
                    }
                    velocity->setX(0.0f);
                    break;
                case BoxCollider::Side::RIGHT:
                    if (values.x == 0.0f) {
                        transform->position.x += 5;
                        break;
                    }
                    velocity->setX(0.0f);
                    break;
                default:
                    break;
            }
        }
    }
}

void System::velocity_system(Entity *e)
{
    auto velocity = e->getComponent<Velocity<float>>();
    auto transform = e->getComponent<Transform2D>();
    auto box = e->getComponent<BoxCollider>();

    if (!velocity || !transform)
        return;
    auto values = velocity->values();
    if (box != nullptr) {
        handle_velocity_colliding_sides(box, transform, velocity);
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

void System::clear_component_cache(const std::vector<IComponent *> &componentCache)
{
    for (auto it : componentCache) {
        delete it;
        it = nullptr;
    }
}

void System::systems()
{
    std::vector<IComponent *> componentCache;

    // Handle hashGrid moving entity
    for (auto e : _dynamic_collider) {
        _hashGrid->insert(e);
    }
    for (auto e : _registry) {
        camera_system(e);
        auto sprite = e->getComponent<Sprite>();

        if (System::lightSources > 0 && !Light::set && sprite) {
            sprite->setColor(Light::darkColor);
        }
        if (!isInView(e))
            continue;
        _inView.push_back(e);

        // Test
        update_custom_component(e);
        sprite_system(e, componentCache);
        light_system(e);
        gravity_system(e);
        BoxSystem(e);
        collider_system(e);
        velocity_system(e);
    }
    clear_component_cache(componentCache);
    componentCache.clear();
    _hashGrid->clear();
    Light::set = true;
}

void System::handle_sprite_lightning(Sprite *sprite, Light *light)
{
    if (sprite && !sprite->isLightApply() && lightSources > 0) {
        auto color = Light::darkColor;
        auto spriteIntensity = sprite->getShadowIntensity();

        if (spriteIntensity != .4f) {
            color = Light::loadColorFromIntensity(spriteIntensity);
            sprite->setColor(color);
        } else
            sprite->setColor(color);
    }
}

bool System::light_layer_raycast(Light *light, Entity *e)
{
    if (_layers.size() > 0) {
        for (auto layer : _layers) {
            if (!layer->contain(e))
                continue;
            if (!light->isSpriteLoaded()) {
                std::vector<Entity *> entities = layer->checkEdges<Transform2D>(
                        e,light->getEmission() / layer->tileWidth);
                light->emit(entities);
                continue;
            }
            light->emit();
        }
        return true;
    }
    return false;
}

void System::light_system(Entity *e)
{
    auto light = e->getComponent<Light>();
    auto sprite = e->getComponent<Sprite>();

    handle_sprite_lightning(sprite, light);
    if (!light) return;
    if (!isInView(e)) return;
    if (light_layer_raycast(light, e))
        return;
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

bool System::resolution_calculation(BoxCollider *box, BoxCollider *collider, Entity *entity)
{
    box->isColliding = true;
    box->collidingWith = entity;
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
        return false;
    }
    (fixedPos1X < fixedPos2X) ? box->registerSide(BoxCollider::Side::LEFT) : box->registerSide(BoxCollider::Side::RIGHT);
    (fixedPos1Y < fixedPos2Y) ? box->registerSide(BoxCollider::Side::DOWN) : box->registerSide(BoxCollider::Side::TOP);
    return true;
}

void System::collision_resolution(BoxCollider *box, BoxCollider *collider)
{
    auto entity = collider->entity();

    box->collide = (collider->overlap(box)) ? BoxCollider::Collide::TRUE : BoxCollider::Collide::FALSE;
    // Resolve collision
    if (box->collide) {
        if (!resolution_calculation(box, collider, entity))
            return;

        // Call Collider Systems
        auto systems = box->getColliderSystem();

        for (auto &system : systems) {
            system(collider);
        }
    }
}

void System::handle_layer_collision(BoxCollider *box, int range, Entity *e)
{
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
        for (auto entity : arr) {
            auto collider = entity->getComponent<BoxCollider>();
            collision_resolution(box, collider);
        }
        box->collide = (box->getSides().size() > 0) ? BoxCollider::Collide::TRUE : BoxCollider::Collide::FALSE;
    }
}

void System::handle_dynamic_entity_collision(Entity *e, BoxCollider *box)
{
    std::vector<Entity *> dynamic_entity = _hashGrid->retrieve(e);

    for (size_t i = 0; i < dynamic_entity.size(); i++) {
        auto d_e = dynamic_entity[i];

        if (d_e == e)
            continue;
        if (d_e == nullptr) {
            _dynamic_collider.erase(std::remove(
                                            _dynamic_collider.begin(),
                                            _dynamic_collider.end(), d_e),
                                    _dynamic_collider.end());
            continue;

        }
        auto other = d_e->getComponent<BoxCollider>();
        collision_resolution(box, other);
        box->collide = (box->getSides().size() > 0) ? BoxCollider::Collide::TRUE : BoxCollider::Collide::FALSE;
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
    box->isColliding = false;
    box->collidingWith = nullptr;
    range = box->getRange();
    if (range == 0)
        return;
    // check collision with layers
    handle_layer_collision(box, range, e);

    // check collision with other dynamic entity
    handle_dynamic_entity_collision(e, box);
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
