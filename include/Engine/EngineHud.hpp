#pragma once
#include "Entity.hpp"
#include <vector>

class EngineHud {
public:
    EngineHud() = default;
    ~EngineHud() = default;

    void entityWindow(const std::vector<Entity *>& _registry);
};