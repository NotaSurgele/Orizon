#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class IEntity {
    public:
        virtual void setPosition(sf::Vector2<float> const &position) = 0;
        virtual void setSize(sf::Vector2<float> const &size) = 0;
        virtual void setSprite(sf::Sprite const& sprite) = 0;
        virtual void setTexture(sf::Texture const& texture) = 0;

    private:

};
