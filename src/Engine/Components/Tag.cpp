#include "Tag.hpp"
#include "Core.hpp"

Tag::Tag(Entity *e, const std::string &value) : _signature(value)
{
    R_ADD_TAG(value);
}

void Tag::setValue(const std::string &v)
{
    auto old = _signature;
    _signature = v;
    auto nb = R_SET_TAG(v, old);
    if (nb > 0) _signature += std::to_string(nb);
}