#pragma once

#define SIGNATURE(type) \
    typeid(type).name()

class Entity;

class IComponent {
  public:
    IComponent() = default;
    ~IComponent() = default;

    virtual void destroy() = 0;
};
