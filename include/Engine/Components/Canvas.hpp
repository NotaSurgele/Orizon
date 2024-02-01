#pragma once
#include "IComponent.hpp"
#include "Entity.hpp"
#include "Sprite.hpp"
#include "Drawable.hpp"
#include <SFML/Graphics.hpp>

class CanvasObject {
public:
    CanvasObject() = default;
    ~CanvasObject() = default;

    enum CoordType {
        WORLD,
        LOCAL
    };

public:
    CoordType type = LOCAL;

};

class Text : public sf::Text, public CanvasObject {
public:
    Text() = default;
};

class Button : public Drawable, public CanvasObject {
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size, sf::Texture& texture);
    ~Button() override = default;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        _sprite->draw(target, states);
    }

    void setTexture(sf::Texture& texture)
    {
        _sprite->setTexture(texture);
    }

    void setTexture(sf::Texture& texture, const std::string& name)
    {
        _sprite->setTexture(texture);
        _sprite->setTextureName(name);
    }

    void setCallback(const std::function<void()>& callback)
    {
        _callback = callback;
    }

    void call()
    {
        try {
            _callback();
        } catch (std::exception& err) {
            std::cerr << "[CANVAS] Callback method is not defined " << err.what() << std::endl;
        }
    }

    void setPosition(const float& x, const float& y)
    {
        _position.x = x;
        _position.y = y;
        _sprite->setPosition(x, y);
        sf::RectangleShape shape = sf::RectangleShape();
    }

    void setPosition(const sf::Vector2f& position)
    {
        _position = position;
        _sprite->setPosition(position);
    }

    sf::Vector2f getPosition()
    {
        return _position;
    }

    sf::Vector2f getSize()
    {
        return _size;
    }

    sf::Vector2u getTextureSize()
    {
        auto textureSize = _sprite->getTexture()->getSize();

        return {textureSize.x * (unsigned int)_size.x, textureSize.y * (unsigned int)_size.y};
    }

    bool isHovered();

    bool isClicked();

    enum States {
        HOVERED,
        PRESSED,
        NOTHING
    };

public:
    States state = NOTHING;

private:
    Sprite *_sprite;
    sf::Vector2f _position;
    sf::Vector2f _size;
    std::function<void()> _callback;
};

class Canvas : public IComponent {
public:
    explicit Canvas(Entity *e);
    ~Canvas() = default;

    Text *addText(const std::string& content, const sf::Vector2f& pos, const std::size_t& size,
                                                        const sf::Color& color=sf::Color::White);

    Button *addButton(const sf::Vector2f& position, const sf::Vector2f& scale, sf::Texture& texture);

    template <typename T>
    void removeObject(const T& obj)
    {
        remove<T>(obj);
    }

    std::unordered_map<Text *, sf::Vector2f>& getTexts()
    {
        return _text;
    }

    std::unordered_map<Button *, sf::Vector2f> getButtons()
    {
        return _button;
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
    std::unordered_map<Button *, sf::Vector2f> _button;
    Entity *_e = nullptr;
};