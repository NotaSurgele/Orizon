#include "QuadTree.hpp"
#include "Engine/Components/BoxCollider.hpp"

QuadTree::QuadTree(int level, float x, float y, float width, float height, int capacity)
: m_level(level), m_bounds({x, y, width, height}), m_capacity(capacity) {
    for (int i = 0; i < 4; i++) {
        m_nodes[i] = nullptr;
    }
}

QuadTree::~QuadTree() {
    clear();
}

void QuadTree::clear() {
    m_entities.clear();

    for (int i = 0; i < 4; i++) {
        if (m_nodes[i] != nullptr) {
            delete m_nodes[i];
            m_nodes[i] = nullptr;
        }
    }
}

void QuadTree::insert(Entity* entity) {
    if (!m_bounds.contains(entity->getComponent<Transform2D>()->position)) {
        return;
    }

    if (m_entities.size() < m_capacity || m_level == MAX_LEVELS) {
        m_entities.push_back(entity);
    } else {
        if (m_nodes[0] == nullptr) {
            split();
        }

        int quadrant = getQuadrant(entity->getComponent<Transform2D>()->position.x, entity->getComponent<Transform2D>()->position.y);
        m_nodes[quadrant]->insert(entity);
    }
}

void QuadTree::remove(Entity* entity) {
    auto it = std::find(m_entities.begin(), m_entities.end(), entity);
    if (it != m_entities.end()) {
        m_entities.erase(it);
        return;
    }

    if (m_nodes[0] != nullptr) {
        int quadrant = getQuadrant(entity->getComponent<Transform2D>()->position.x, entity->getComponent<Transform2D>()->position.y);
        m_nodes[quadrant]->remove(entity);
    }
}

int QuadTree::getQuadrant(float x, float y) {
    int index = -1;
    float verticalMidpoint = m_bounds.left + (m_bounds.width / 2.f);
    float horizontalMidpoint = m_bounds.top + (m_bounds.height / 2.f);

    // Check if point is in the left or right quadrants
    bool isLeft = x < verticalMidpoint;
    bool isTop = y < horizontalMidpoint;

    if (isLeft) {
        if (isTop) {
            index = 1; // Top left quadrant
        }
        else {
            index = 2; // Bottom left quadrant
        }
    }
    else {
        if (isTop) {
            index = 0; // Top right quadrant
        }
        else {
            index = 3; // Bottom right quadrant
        }
    }

    return index;
}

std::vector<Entity*> QuadTree::retrieve(Entity* entity) {
    std::vector<Entity*> result;
    BoxCollider *collider = entity->getComponent<BoxCollider>();
    sf::Vector2<float> position = collider->getPosition();
    sf::Vector2<float> size = collider->getSize();

    sf::FloatRect bounds = sf::FloatRect(position.x, position.y, size.x, size.y);
    if (!m_bounds.intersects(bounds)) {
        return result;
    }

    for (auto e : m_entities) {
        if (e != entity) {
            result.push_back(e);
        }
    }

    if (m_nodes[0] != nullptr) {
        int quadrant = getQuadrant(bounds.left, bounds.top);
        std::vector<Entity*> subresult = m_nodes[quadrant]->retrieve(entity);
        result.insert(result.end(), subresult.begin(), subresult.end());
    }

    return result;
}

void QuadTree::split() {
    float subWidth = m_bounds.width / 2.f;
    float subHeight = m_bounds.height / 2.f;
    float x = m_bounds.left;
    float y = m_bounds.top;

    m_nodes[0] = new QuadTree(m_level + 1, x + subWidth, y, subWidth, subHeight, m_capacity);
    m_nodes[1] = new QuadTree(m_level + 1, x, y, subWidth, subHeight, m_capacity);
    m_nodes[2] = new QuadTree(m_level + 1, x, y + subHeight, subWidth, subHeight, m_capacity);
    m_nodes[3] = new QuadTree(m_level + 1, x + subWidth, y + subHeight, subWidth, subHeight, m_capacity);

    for (auto e : m_entities) {
        Transform2D *transform = e->getComponent<Transform2D>();
        int quadrant = getQuadrant(transform->position.x, transform->position.y);
        m_nodes[quadrant]->insert(e);
    }

    m_entities.clear();
}