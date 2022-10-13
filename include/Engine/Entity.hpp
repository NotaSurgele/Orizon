#include "IEntity.hpp"
#include <unordered_map>
#include <string>

class Entity : public IEntity {
    public:
        Entity() = default;
        ~Entity() = default;

        void setPosition(sf::Vector2<float> const& position) override;
        void setSize(sf::Vector2<float> const& size) override;
        void setSprite(sf::Sprite const& sprite) override;
        void setTexture(sf::Texture const& texture) override;

    protected:
        sf::Sprite _sprite;
        sf::Texture _texture;
        sf::Vector2<float> _position;
        sf::Vector2<float> _size;
};
