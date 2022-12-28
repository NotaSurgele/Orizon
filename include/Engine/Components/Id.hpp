#pragma once
#include "IComponent.hpp"

class Id : public IComponent {
public:
    Id(Entity *e, std::size_t const& id) : __id(id)
    {
    }

    ~Id() = default;

    const std::size_t& get_id()
    {
        return __id;
    }

    void destroy() override final
    {
    }

    private:
        const std::size_t __id;
};
