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

    void setOffset(const float& x, const float& y)
    {
        _offset.x = x;
        _offset.y = y;
    }

    void setOffset(const sf::Vector2f& offset)
    {
        _offset = offset;
    }

    sf::Vector2f getOffset()
    {
        return _offset;
    }

    void setBasePosition(const float& x, const float& y)
    {
        _basePosition.x = x;
        _basePosition.y = y;
    }

    void setSave(const bool& save)
    {
        _save = save;
    }

    bool shouldSave()
    {
        return _save;
    }

    virtual void setZ(const int& index) = 0;

    const int getZ()
    {
        return _z;
    }

    void setBasePosition(const sf::Vector2f& base)
    {
        _basePosition = base;
    }

    sf::Vector2f& getBasePosition()
    {
        return _basePosition;
    }

    enum CoordType {
        LOCAL,
        WORLD
    };

public:
    CoordType coordType = LOCAL;

protected:
    bool _save = true;
    sf::Vector2f _offset = {0, 0};
    sf::Vector2f _basePosition = {0, 0};
    int _z = 0;
};

class Text : public sf::Text, public CanvasObject {
public:
    Text() = default;

    Text(const std::string& content, const sf::Font& font, const std::size_t& size) : sf::Text(content, font, size)
    {
    }

    void setZ(const int& index) override final;

};

class Image : public Drawable, public CanvasObject {
public:
    explicit Image(sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& scale,
                        const sf::Color& color=sf::Color::White) : _image(new Sprite(texture))
    {
        _image->setScale(scale.x, scale.y);
        _image->setPosition(position);
        _image->setColor(color);
    }

    ~Image() = default;

    void setPosition(const sf::Vector2f& position)
    {
        _image->setPosition(position);
    }

    void setPosition(const float& x, const float& y)
    {
        _image->setPosition(x, y);
    }

    sf::Vector2f getPosition()
    {
        return _image->getPosition();
    }

    sf::Vector2u getTextureSize()
    {
        return _image->getTexture()->getSize();
    }

    void setSize(const sf::Vector2f& size)
    {
        _image->setScale(size.x, size.y);
    }

    Sprite *getImage()
    {
        return _image;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        _image->draw(target, states);
    }

    void setZ(const int& index) override final;

private:
    Sprite *_image;
};

class Button : public Drawable, public CanvasObject {
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size, sf::Texture texture);
    ~Button() override = default;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        _sprite->draw(target, states);
    }

    void setTexture(sf::Texture& texture)
    {
        _sprite->setTexture(texture);
    }

    Sprite* getSprite()
    {
        return _sprite;
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

    void setText(const std::string& content, const std::size_t& fontSize, const sf::Color& color=sf::Color::White)
    {
        _font = sf::Font();
        float padding = 50.0f;

        if (!_font.loadFromFile("../assets/LEMONMILK-Regular.otf")) {
            std::cout << "[BUTTON] Cannot load font " << std::endl;
            return;
        }

        text = new Text(content, _font, fontSize);

        text->setFillColor(color);
/*        auto spriteBounds = _sprite->getGlobalBounds();
        auto textBounds = text->getGlobalBounds();

        float scaleX = (textBounds.width + padding) / spriteBounds.width;
        float scaleY = (textBounds.height + padding) / spriteBounds.height;

        _sprite->setScale(scaleX, scaleY);*/
    }

    void call()
    {
        try {
            _callback();
        } catch (std::exception& err) {
            std::cerr << "[CANVAS] Callback method is not defined " << err.what() << std::endl;
        }
    }

    void setScale(const float& x, const float& y)
    {
        _size.x = x;
        _size.y = y;
        _sprite->setScale(x,y);
    }

    void setScale(const sf::Vector2f& size)
    {
        _size = size;
        _sprite->setScale(_size.x, _size.y);
    }

    void setPosition(const float& x, const float& y)
    {
        _position.x = x;
        _position.y = y;
        _sprite->setPosition(x, y);
    }

    void setPosition(const sf::Vector2f& position)
    {
        _position = position;
        _sprite->setPosition(position);
    }

    void setZ(const int& index) override final;

    std::string getTextContent()
    {
        if (text) {
            return text->getString();
        }
        return "";
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
    Text *text = nullptr;

private:
    sf::Font _font;
    Sprite *_sprite;
    sf::Vector2f _position;
    sf::Vector2f _size;
    std::function<void()> _callback;
};

class Canvas : public IComponent {
public:
    explicit Canvas(Entity *e);
    ~Canvas() = default;

    Text *addText(const std::string& content, const sf::Vector2f& pos, const std::size_t& size, bool save=true);

    Button *addButton(const sf::Vector2f& position, const sf::Vector2f& scale, sf::Texture texture, bool save=true);


    Image *addImage(sf::Texture texture, const sf::Vector2f& position,
                     const sf::Vector2f& scale, bool save=true)
    {
        auto img = new Image(texture, position, scale);

        img->setSave(save);
        img->setBasePosition(position);
        _image.push_back(img);
        return img;
    }


    template <typename T>
    void removeObject(T obj)
    {
        remove<T>(obj);
    }

    template <typename T>
    void sort() {}

    std::vector<Text *> getTexts()
    {
        return _text;
    }

    std::vector<Button *> getButtons()
    {
        return _button;
    }

    std::vector<Image *> getImages()
    {
        return _image;
    }

    void destroy() final;

public:
    static inline bool buttonSorted = true;
    static inline bool textSorted = true;
    static inline bool imageSorted = true;

private:
    template <typename T>
    void remove(T obj);

private:
    sf::Font _font;
    std::vector<Text *> _text;
    std::vector<Button *> _button;
    std::vector<Image *> _image;
    Entity *_e = nullptr;
};

template <typename T>
void sortTemplate(std::vector<T>& array)
{
    std::sort(array.begin(), array.end(),
      [](T first, T second) {
          auto f = first->getZ();
          auto s = second->getZ();

          return f < s;
      }
    );
}

template <>
inline void Canvas::sort<Text *>()
{
    sortTemplate<Text *>(_text);
}

template <>
inline void Canvas::sort<Button *>()
{
    sortTemplate<Button *>(_button);
}

template <>
inline void Canvas::sort<Image *>()
{
    sortTemplate<Image *>(_image);
}

template <>
inline void Canvas::remove<Text *>(Text *text) {
    _text.erase(std::remove(_text.begin(), _text.end(), text), _text.end());
}

template <>
inline void Canvas::remove<Button *>(Button *button) {
    _button.erase(std::remove(_button.begin(), _button.end(), button), _button.end());
}

template <>
inline void Canvas::remove<Image *>(Image *img) {
    _image.erase(std::remove(_image.begin(), _image.end(), img), _image.end());
}

template <typename T>
inline void Canvas::remove(T obj) {
    remove<T>(obj);
}

