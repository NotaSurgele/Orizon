#pragma once

#include <map>
#include <functional>
#include <string>

#define SIGNATURE(type) \
    typeid(type).name()

class Entity;
class Transform2D;

class IComponent {
  public:
    IComponent() = default;
    ~IComponent() = default;

    virtual void destroy() = 0;
};

template <class... Args>
class ComponentFactory {
    public:

        static std::pair<const char *, IComponent *> create_component(std::string const &name, Entity *e, Args... args)
        {
            for (auto &it : _construct_map) {
                if (it.first == name)
                    return it.second(e, args ...);
            }
        }

        static std::pair<const char *, IComponent *> create_transform(Entity *e, Args ... args)
        {
            Transform2D *transform = new Transform2D(e, args ...);
            std::string signature = SIGNATURE(Transform2D);

            return std::make_pair<const char *, IComponent *>(signature.c_str(), transform);
        }

    private:
        static inline std::unordered_map<const char *, std::function<void>(Entity *e, Args...)> _construct_map = {
            { "Transform2D", &create_transform }
        };
};