#include "Engine/System.hpp"
#include "Components/View.hpp"
#include "Components/Light.hpp"
#include "Core.hpp"
#include "RayCaster.hpp"
#include "Script.hpp"
#include "Canvas.hpp"

void System::addEntity(Entity *entity)
{
    entity->addComponent<Id>(_id++);
    entity->addComponent<Layer>(0);
    Light::set = false;
}

void System::pushEntity(Entity *entity)
{
    if (!entity) {
        std::cerr << "[SYSTEM] Cannot push entity because entity is NULL" << std::endl;
        return;
    }
    auto l = entity->getComponent<Layer>();

    if (!l) {
        std::cerr << "PUSH ENTITY " << "THIS SHOULD NOT HAPPEN" << std::endl;
        return;
    }
    _registry_size = _registry.size();
    ___insert_entity_at_location(entity);
    auto value = l->value();
    auto position = _orders_values[value];
    if (position >= _registry_size) {
        _registry.push_back(entity);
        entity->__registryPosition = _registry_size + 1;
    }
    else {
        _registry.insert(_registry.begin() + position, entity);
        entity->__registryPosition = position;
    }
}

void System::forceDestroy()
{
    for (auto& e : _to_destroy) {
        if (!e) continue;
        if (e->hasComponents<Light>()) __removeLightSource(e);
        _dynamic_collider.erase(std::remove(
                                        _dynamic_collider.begin(),
                                        _dynamic_collider.end(),
                                        e),
                                _dynamic_collider.end());
        _forceUpdate.erase(std::remove(_forceUpdate.begin(), _forceUpdate.end(), e), _forceUpdate.end());
        for (auto& layer : _layers) {
            if (layer->contain(e)) {
                layer->removeEntity(e);
            }
        }
        e->__destroyComponents();
        e = nullptr;
        // delete e; [TODO] Fix this
    }
    for (auto& l : _destroy_tilemap) {
        l->clear();

        _layers.erase(std::remove(_layers.begin(), _layers.end(), l), _layers.end());
    }
    _destroy_tilemap.clear();
    _to_destroy.clear();
    _hashGrid->clear();
}

void System::forceUpdate(Entity *e)
{
    _forceUpdate.push_back(e);
}

void System::handleVelocityCollidingSides(BoxCollider *box, Transform2D *transform, Velocity *velocity)
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

void System::velocitySystem(Entity *e)
{
    auto velocity = e->getComponent<Velocity>();
    auto transform = e->getComponent<Transform2D>();
    auto boxes = e->getComponents<BoxCollider>();

    if (!velocity || !transform)
        return;
    for (auto& box : boxes) {
        if (box != nullptr) {
            handleVelocityCollidingSides(box, transform, velocity);
        }
    }
    transform->position.x += velocity->getX() * Time::deltaTime;
    transform->position.y += velocity->getY() * Time::deltaTime;
}

void System::sort()
{
    std::sort(_registry.begin(), _registry.end(),
        [](Entity *e1, Entity *e2) {
            auto l1 =  e1->getComponent<Layer>();
            auto l2 = e2->getComponent<Layer>();

            if (!l1 || !l2)
                return true;
            auto v1 = l1->value();
            auto v2 = l2->value();

            return v1 > v2;
        }
    );
}

void System::spriteSystem(Entity *e, std::vector<IComponent *> componentCache)
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

void System::canvasSystem(Entity *e)
{
    auto canvas = e->getComponent<Canvas>();
    auto texts = canvas->getTexts();

    // Sort each canvas registry
    if (!Canvas::textSorted) {
        canvas->sort<Text *>();
        Canvas::textSorted = true;
    }

    if (!Canvas::buttonSorted) {
        canvas->sort<Button *>();
        Canvas::buttonSorted = true;
    }

    if (!Canvas::imageSorted) {
        canvas->sort<Image *>();
        Canvas::imageSorted = true;
    }

    // Text system
    for (auto& t : texts) {
        auto offset = t->getOffset();
        auto& position = t->getBasePosition();

        if (t->coordType == Text::LOCAL) {
            auto v = WindowInstance.getView();
            auto center = v->getCenter();
            sf::FloatRect textBounds = t->getLocalBounds();

            t->setPosition(((position.x + offset.x) + center.x) - (textBounds.width / 2),
                           ((position.y + offset.y) + center.y) - (textBounds.height / 2));
        } else t->setPosition(position.x + offset.x, position.y + offset.y);
        DRAW(*t);
    }


    // Button system
    auto buttons = canvas->getButtons();
    bool isHovered = false;

    for (auto& b : buttons) {
        auto offset = b->getOffset();
        auto& text = b->text;
        auto& position = b->getBasePosition();

        if (b->coordType == Text::LOCAL) {
            auto v = WindowInstance.getView();
            auto center = v->getCenter();

            b->setPosition(((position.x + offset.x) + center.x),
                            ((position.y + offset.y) + center.y));
        } else b->setPosition(position.x + offset.x, position.y + offset.y);

        if (!isHovered) {
            auto mousePos = getGlobalMousePosition();
            auto bounds = b->getSprite()->getGlobalBounds();

            if (bounds.contains((float)mousePos.x, (float)mousePos.y)) {
                isHovered = true;
                b->state = Button::HOVERED;

                if (b->isClicked()) {
                    b->state = Button::PRESSED;
                    b->call();
                }
            } else b->state = Button::NOTHING;
        } else b->state = Button::NOTHING;
        DRAW(b);

        //Handle button text
        if (text != nullptr) {
            auto spriteBounds = b->getSprite()->getGlobalBounds();
            text->setPosition(spriteBounds.left, spriteBounds.top);
            DRAW(*text);
        }
    }

    auto images = canvas->getImages();
    // Images system
    for (auto& i : images) {
        auto offset = i->getOffset();
        auto& position = i->getBasePosition();

        if (i->coordType == Text::LOCAL) {
            auto v = WindowInstance.getView();
            auto center = v->getCenter();
            auto size = i->getTextureSize();

            i->setPosition(((position.x + offset.x) + center.x) - ((float)size.x / 2),
                           ((position.y + offset.y) + center.y) - ((float)size.y / 2));
        } else i->setPosition(position.x + offset.x, position.y + offset.y);
        DRAW(i);
    }
}

void System::scriptSystem(Entity *e)
{
    std::vector<Script *> scriptArray = e->getComponents<Script>();

    for (auto& s : scriptArray) {
        s->start();
        s->update();
    }
}

void System::clearComponentCache(const std::vector<IComponent *> &componentCache)
{
    //EngineHud::writeConsole<std::string, std::size_t>("Component cache size", componentCache.size());
    for (auto it : componentCache) {
        delete it;
        it = nullptr;
    }
}

sf::Vector2f System::getLocalMousePosition()
{
    auto pos = sf::Mouse::getPosition();
    return {(float)pos.x, (float)pos.y};
}

sf::Vector2f System::getGlobalMousePosition()
{
    return WindowInstance.mapPixelToCoords(sf::Mouse::getPosition(*WindowInstance.getSFMLRenderWindow()));
}

sf::Vector2f System::localToGlobalCoordinate(const sf::Vector2f& local)
{
    return WindowInstance.mapPixelToCoords((sf::Vector2i)local);
}

sf::Vector2f System::globalToLocalCoordinate(const sf::Vector2f &global)
{
    return (sf::Vector2f)WindowInstance.mapCoordsToPixel(global);
}

void System::systems()
{
    std::vector<IComponent *> componentCache;

    // Handle force update entity
    for (auto& e : _forceUpdate) {
        pushEntity(e);
    }

    // Go through layers
    for (auto& m : _layers) {
        if (!m->isRender() || !isInView(m)) continue;
        auto entities = m->getEntityInBounds(WindowInstance.getView()->getViewBounds());

        for (auto& e : entities) {
            pushEntity(e);
        }
    }

    //EngineHud::writeConsole<std::string, std::size_t>("Inside dynamic collider ", _dynamic_collider.size());
    // Handle hashGrid moving entity
    EngineHud::writeConsole<std::string, std::size_t>("Dynamic collider ", _dynamic_collider.size());
    for (auto e : _dynamic_collider) {
        if (!e) continue;
        _hashGrid->insert(e);
    }

    for (auto e : _registry) {
        if (!e) continue;

        cameraSystem(e);
        setSpriteShadow(e);
        updateCustomComponent(e);
        spriteSystem(e, componentCache);
        gravitySystem(e);
        BoxSystem(e);
        colliderSystem(e);
        velocitySystem(e);
    }

    // Light source system
    for (auto& e : _lightSource) {
        lightSystem(e);
    }

    for (auto& e : _canvas) {
        canvasSystem(e);
    }

    // Handle entity with script
    for (auto& e : _scripted_entity) {
        scriptSystem(e);
    }
    //EngineHud::writeConsole<std::string, std::size_t>("the size of the registry is ", _registry.size());
    clearComponentCache(componentCache);
    componentCache.clear();
    destroyEntity();
    _registry.clear();
    _orders_values.clear();
    Light::set = true;
}

void System::handleSpriteLightning(Sprite *sprite, Light *light)
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

bool System::lightLayerRaycast(Light *light, Entity *e)
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

void System::setSpriteShadow(Entity *e)
{
    auto sprite = e->getComponent<Sprite>();

    if (!sprite) return;
    if (lightSources <= 0) {
        sprite->setColor(sf::Color::White);
        return;
    }
    sprite->setColor(Light::darkColor);
}

void System::lightSystem(Entity *e)
{
    auto arr = e->getComponents<Light>();

    for (auto& light : arr) {
        auto sprite = e->getComponent<Sprite>();

        if (!light) return;
        handleSpriteLightning(sprite, light);
        if (lightLayerRaycast(light, e))
            return;
        if (!light->isSpriteLoaded()) {
            light->emit(_registry);
            return;
        }
        light->emit();
    }
}

void System::gravitySystem(Entity *e)
{
    auto velocity = e->getComponent<Velocity>();
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

void System::updateCustomComponent(Entity *e)
{
    auto map = e->getCustomComponents();

    for (auto &it2 : map) {
        auto &component = it2.second;

        component->update();
    }
}

bool System::resolutionCalculation(BoxCollider *box, BoxCollider *collider, Entity *entity)
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

void System::collisionResolution(BoxCollider *box, BoxCollider *collider)
{
    auto entity = collider->entity();

    box->collide = (collider->overlap(box)) ? BoxCollider::Collide::TRUE : BoxCollider::Collide::FALSE;
    // Resolve collision
    if (box->collide) {
        if (!resolutionCalculation(box, collider, entity))
            return;

        // Call Collider Systems
        auto systems = box->getColliderSystem();

        for (auto &system : systems) {
            system(collider);
        }
    }
}

void System::handleLayerCollision(BoxCollider *box, int range, Entity *e)
{
    box->setColor(sf::Color::Green);
    box->sides.clear();
    box->side = BoxCollider::Side::NONE;
    box->collide = BoxCollider::Collide::FALSE;
    for (TileMap *layer : _layers) {
        float x = box->getPosition().x;
        float y = box->getPosition().y;

        if (!layer->isRender() || !layer->contain(x, y))
            continue;
        std::vector<Entity *> arr = layer->checkAround<BoxCollider>(e, range);
        for (auto entity : arr) {
            auto collider = entity->getComponent<BoxCollider>();
            collisionResolution(box, collider);
        }
        box->collide = (!box->getSides().empty()) ? BoxCollider::Collide::TRUE : BoxCollider::Collide::FALSE;
    }
}

void System::handleDynamicEntityCollision(Entity *e, BoxCollider *box)
{
    std::vector<Entity *> dynamic_entity = _hashGrid->retrieve(e);

    for (auto d_e : dynamic_entity) {
        if (d_e == e)
            continue;
        if (d_e == nullptr) {
            _dynamic_collider.erase(std::remove(
                                            _dynamic_collider.begin(),
                                            _dynamic_collider.end(), d_e),
                                    _dynamic_collider.end());
            continue;

        }
        auto arr = d_e->getComponents<BoxCollider>();

        for (auto& other : arr) {
            collisionResolution(box, other);
            box->collide = (!box->getSides().empty()) ? BoxCollider::Collide::TRUE : BoxCollider::Collide::FALSE;
        }
    }
}

void System::colliderSystem(Entity *e)
{
    auto arr = e->getComponents<BoxCollider>();

    for (auto& box : arr) {
        int range = 0;

        if (box == nullptr)
            return;
        if (box->getType() == BoxCollider::Type::STATIC)
            return;
        /*if (!box->___isSet) {
            _dynamic_collider.push_back(e);
            box->___isSet = true;
        }*/
        box->isColliding = false;
        box->collidingWith = nullptr;
        range = box->getRange();
        if (range == 0) return;
        // check collision with layers
        handleLayerCollision(box, range, e);

        // check collision with other dynamic entity
        handleDynamicEntityCollision(e, box);
    }
}

void System::cameraSystem(Entity *e) {
    auto view = e->getComponent<View>();
    auto transform = e->getComponent<Transform2D>();
    bool destroy = false;

    if (!view)
        return;
    SET_VIEW(view);
    if (!view->isFollowing())
        return;
    if (!transform) {
        destroy = true;
        transform = Transform2D::zero();
    }
    view->setCenter(transform->position);
    if (destroy) {
        transform->destroy();
        destroy = false;
    }
}

void System::BoxSystem(Entity *e)
{
    auto transform = e->getComponent<Transform2D>();
    auto velocity = e->getComponent<Velocity>();
    auto arr = e->getComponents<BoxCollider>();

    for (auto& box : arr) {
        bool d_v = false;

        if (!box)
            return;
        if (!velocity) {
            velocity = Velocity::zero(),
            d_v = true;
        }
        float velX = velocity->getX();
        float velY = velocity->getY();
        float x = (velX > 0) ? 2 : (velX < 0) ? -2 : 0;
        float y = (velY > 0) ? 2 : (velY < 0) ? -2 : 0;
        auto offset = box->getOffset();

        box->setPosition((transform->position.x + offset.x) + x,
                        (transform->position.y + offset.y) + y);
        if (box->shouldDraw()) DRAW(box);
        if (d_v) delete velocity;
    }
}

void System::merge()
{
    systems();
    //quad_collision_system();
}

bool System::isInView(Entity *e)
{
    auto transform = e->getComponent<Transform2D>();
    auto currentView = WindowInstance.getView();

    if (!transform)
        transform = Transform2D::zero();
    if (currentView != nullptr) {
        sf::Vector2f padding(0, 0);

        if (!_layers.empty()) {
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

bool System::isInView(TileMap *map)
{
    auto mapPosition = map->getPosition();
    auto mapSize = map->getSize();
    sf::FloatRect mapBounds = sf::FloatRect(mapPosition, mapSize);
    auto view = WindowInstance.getView();

    return view->getViewBounds().intersects(mapBounds);
}

void System::destroyEntity()
{
    for (auto& e : _to_destroy) {
        if (!e) continue;
        if (e->hasComponents<Light>()) __removeLightSource(e);

        _dynamic_collider.erase(std::remove(
                                        _dynamic_collider.begin(),
                                        _dynamic_collider.end(),
                                        e),
                                _dynamic_collider.end());
        _forceUpdate.erase(std::remove(_forceUpdate.begin(), _forceUpdate.end(), e), _forceUpdate.end());
        for (auto& layer : _layers) {
            if (layer->contain(e)) {
                layer->removeEntity(e);
            }
        }
        e->__destroyComponents();
        //delete e; /*[TODO] Fix this*/
        _registry_size--;
    }
    for (auto& l : _destroy_tilemap) {
        l->clear();

        _layers.erase(std::remove(_layers.begin(), _layers.end(), l), _layers.end());
    }
    _destroy_tilemap.clear();
    _to_destroy.clear();
    _hashGrid->clear();
}
