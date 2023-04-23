#include "QuadTree.hpp"
#include "Engine/Core.hpp"

QuadtreeNode::QuadtreeNode(const sf::FloatRect& boundingBox) : boundingBox(boundingBox), isLeaf(true)
{
    for (int i = 0; i < 4; i++) {
        childNodes[i] = nullptr;
    }
}

QuadtreeNode::~QuadtreeNode()
{
    for (int i = 0; i < 4; i++) {
        delete childNodes[i];
    }
}

void QuadtreeNode::insert(Entity* entity)
{
    rect.setSize(sf::Vector2(boundingBox.width, boundingBox.height));
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Magenta);
    rect.setOutlineThickness(1.0);
    DRAW(rect);
    if (!boundingBox.contains(entity->getComponent<Transform2D>()->position)) {
        return;
    }

    auto it = std::find(entities.begin(), entities.end(), entity);
    if (it != entities.end())
        return;
    if (isLeaf && entities.size() < 4) {
        entities.push_back(entity);
    } else {
        if (isLeaf) {
            split();
        }

        for (int i = 0; i < 4; i++) {
            childNodes[i]->insert(entity);
        }
    }
}

void QuadtreeNode::retrieve(Entity* entity, std::vector<Entity*>& foundEntities)
{
    if (!boundingBox.intersects(sf::FloatRect(entity->getComponent<Transform2D>()->position, sf::Vector2f(1.f, 1.f)))) {
        return;
    }

    if (!isLeaf) {
        for (int i = 0; i < 4; i++) {
            childNodes[i]->retrieve(entity, foundEntities);
        }
    }

    foundEntities.insert(foundEntities.end(), entities.begin(), entities.end());
}

void Quadtree::remove(Entity* entity)
{
    // Remove the entity from this node if it is present
    auto it = std::find(root.entities.begin(), root.entities.end(), entity);
    if (it != root.entities.end()) {
        root.entities.erase(it);
        return;
    }

    // Recursively remove the entity from child nodes if it is present
    if (!root.isLeaf) {
        for (int i = 0; i < 4; i++) {
            auto& childNode = *(root.childNodes[i]);
            auto it = std::find(childNode.entities.begin(), childNode.entities.end(), entity);
            if (it != childNode.entities.end()) {
                childNode.entities.erase(it);
                // If the child node is now empty, merge its child nodes into this node
                if (childNode.isLeaf && childNode.entities.empty()) {
                    for (int j = 0; j < 4; j++) {
                        delete childNode.childNodes[j];
                        childNode.childNodes[j] = nullptr;
                    }
                    childNode.isLeaf = true;
                }
                return;
            }
        }
    }
}

void QuadtreeNode::clear()
{
    entities.clear();
    for (int i = 0; i < 4; i++) {
        if (childNodes[i] != nullptr) {
            childNodes[i]->clear();
            delete childNodes[i];
            childNodes[i] = nullptr;
        }
    }
    isLeaf = true;
}

void QuadtreeNode::split()
{
    float halfWidth = boundingBox.width / 2.f;
    float halfHeight = boundingBox.height / 2.f;

    childNodes[0] = new QuadtreeNode(sf::FloatRect(boundingBox.left, boundingBox.top, halfWidth, halfHeight));
    childNodes[1] = new QuadtreeNode(sf::FloatRect(boundingBox.left + halfWidth, boundingBox.top, halfWidth, halfHeight));
    childNodes[2] = new QuadtreeNode(sf::FloatRect(boundingBox.left, boundingBox.top + halfHeight, halfWidth, halfHeight));
    childNodes[3] = new QuadtreeNode(sf::FloatRect(boundingBox.left + halfWidth, boundingBox.top + halfHeight, halfWidth, halfHeight));
    isLeaf = false;

    for (auto entity : entities) {
        for (int i = 0; i < 4; i++) {
            childNodes[i]->insert(entity);
        }
    }

    entities.clear();
}

Quadtree::Quadtree(const sf::FloatRect& boundingBox) : root(boundingBox) {}

void Quadtree::insert(Entity* entity)
{
    root.insert(entity);
}

void Quadtree::clear()
{
    root.clear();
}

void Quadtree::setBoundingBox(const sf::FloatRect& newBoundingBox)
{
    root = QuadtreeNode(newBoundingBox);
}

std::vector<Entity*> Quadtree::retrieve(Entity* entity)
{
    std::vector<Entity*> foundEntities;
    root.retrieve(entity, foundEntities);
    return foundEntities;
}