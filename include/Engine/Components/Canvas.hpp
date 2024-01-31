#pragma once
#include "IComponent.hpp"
#include "Entity.hpp"
#include <SFML/Graphics.hpp>

class Text : public sf::Text {
public:
    Text() = default;

    enum CoordType {
        WORLD,
        LOCAL
    };

public:
    CoordType type = LOCAL;
};

class Canvas : public IComponent {
public:
    explicit Canvas(Entity *e);
    ~Canvas() = default;

    Text *addText(const std::string& content, const sf::Vector2f& pos, const std::size_t& size,
                                                        const sf::Color& color=sf::Color::White);

    template <typename T>
    void removeObject(const T& obj)
    {
        remove<T>(obj);
    }

    std::unordered_map<Text *, sf::Vector2f>& getTexts()
    {
        return _text;
    }

    void destroy() final;

private:

    template <typename T = Text>
    void remove(Text *text)
    {
        auto find = _text.find(text);
        _text.erase(find);
    }

private:
    sf::Font _font;
    std::unordered_map<Text *, sf::Vector2f> _text;
    Entity *_e = nullptr;
};