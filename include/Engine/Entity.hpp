#include "IEntity.hpp"
#include <unordered_map>
#include <string>

class Entity : public IEntity {
    public:
        Entity() = default;
        ~Entity() = default;

        Entity& setPosition(sf::Vector2<float> const& position) override;
        Entity& setSize(sf::Vector2<float> const& size) override;
        Entity& setSprite(sf::Sprite const& sprite) override;
        Entity& setTexture(sf::Texture const& texture) override;

    protected:
        sf::Sprite _sprite;
        sf::Texture _texture;
        sf::Vector2<float> _position;
        sf::Vector2<float> _size;
};
