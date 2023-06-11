#pragma once

#include "Engine/Entity.hpp"
#include "Raytracer.hpp"

class Light : public IComponent {
public:
    Light(Entity *e);
    void destroy() override final;
    void emit(const std::vector<Entity *>& entities);

private:
    std::vector<RayTracer> _rayCaster;
    Entity *_e = nullptr;
};