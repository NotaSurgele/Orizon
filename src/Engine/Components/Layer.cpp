#include "Layer.hpp"
#include "System.hpp"

void Layer::set(std::size_t const& value) {
    _layer = value;
    System::___insert_entity_at_location(_self);
}
