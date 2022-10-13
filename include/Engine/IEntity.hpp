#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class IEntity {
    public:
        virtual IEntity& setPosition(sf::Vector2<float> const &position) = 0;
        virtual IEntity& setSize(sf::Vector2<float> const &size) = 0;
        virtual IEntity& setSprite(sf::Sprite const& sprite) = 0;
        virtual IEntity& setTexture(sf::Texture const& texture) = 0;

    private:

};
