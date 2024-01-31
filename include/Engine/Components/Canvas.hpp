#pragma once
#include "IComponent.hpp"
#include "Entity.hpp"
#include <SFML/Graphics.hpp>

class Canvas : public IComponent {
public:
    explicit Canvas(Entity *e);
    ~Canvas() = default;

    void addText(const std::string& content, const sf::Vector2f& pos, const std::size_t& size,
                                                        const sf::Color& color=sf::Color::White);

    void addText(sf::Text *text);

    template <typename T>
    void removeObject(const T& obj)
    {
        remove<T>(obj);
    }

    std::vector<sf::Text *>& getTexts()
    {
        return _text;
    }

    void destroy() override final;

private:

    template <typename T = sf::Text>
    void remove(sf::Text *text)
    {
        _text.erase(std::remove(_text.begin(), _text.end(), text), _text.end());
    }

private:
    sf::Font _font;
    std::vector<sf::Text *> _text;
    Entity *_e = nullptr;
};