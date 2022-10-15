#pragma once

#define SIGNATURE(type) \
    typeid(type).name()

class IComponent {
    public:
        IComponent() = default;
        ~IComponent() = default;

        virtual void update() = 0;
};