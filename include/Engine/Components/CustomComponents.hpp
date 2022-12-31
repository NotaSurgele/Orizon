#pragma once

#include "IComponent.hpp"

class CustomComponents : public IComponent {
    public:
        CustomComponents() = default;
        ~CustomComponents() = default;

        virtual void update() = 0;
        virtual void destroy() = 0;
};