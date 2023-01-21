#include "QuadTree.hpp"
#include "Core.hpp"

void QuadTree::collide()
{

}

void QuadTree::show()
{
    std::cout << "Type: " << _type << std::endl;
    std::cout << "  Rectangle: " << _r._x << ", " << _r._y << " " << _r._w << ", " << _r._h << std::endl;
    for (auto& p : _points) {
        auto t = p->getComponent<Transform2D>();
        std::cout << "      Point: " << t->position.x << " " << t->position.y << std::endl;
    }
    for (auto& it : _quads)
        it->show();
}