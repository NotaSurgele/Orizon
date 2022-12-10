#pragma once

#include "IComponent.hpp"

#define EMPTY_SIGNATURE\
        typeid(EmptyComponent).name()

class EmptyComponent : public IComponent {
public:
    EmptyComponent() = default;
    ~EmptyComponent() = default;

    void destroy() override final {}
};
