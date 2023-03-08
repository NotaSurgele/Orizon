#include "QuadTree.hpp"
#include "Core.hpp"

void QuadTree::collide(System& system)
{
    for (auto &e : _points) {
        auto box = e->getComponent<BoxCollider>();
        auto v = e->getComponent<Velocity<float>>();
        bool d_v = false;

        if (!v) d_v = true, v = Velocity<float>::zero();
        for (auto &e2 : _points) {
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
    for (auto& it : _quads) {
        if (it != nullptr)
            it->collide(system);
    }
}

void QuadTree::show()
{
    // std::cout << "Type: " << _type << std::endl;
    // std::cout << "  Rectangle: " << _r._x << ", " << _r._y << " " << _r._w << ", " << _r._h << std::endl;
    // for (auto& p : _points) {
    //     auto t = p->getComponent<Transform2D>();
    //     auto box = p->getComponent<BoxCollider>();
    //     // std::cout << "      Point: " << t->position.x << " " << t->position.y << std::endl;
    //     DRAW(box);
    // }
    // for (auto& it : _quads) {
    //     if (it != nullptr)
    //         it->show();
    // }
}