#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "Components/View.hpp"
#include "Engine/Components/Sprite.hpp"
#include "Engine/Components/Drawable.hpp"

#define DRAW_NULL_ERROR "Error [RenderWindow] draw method: pointer is null 0x0" \
                        " cannot draw"
class RenderWindow {
    public:
        RenderWindow(std::string const& windowName,
                std::size_t width = 800,
                std::size_t height = 600);
        RenderWindow() = default;
        ~RenderWindow() = default;

        sf::Vector2f mapPixelToCoords(const sf::Vector2i& point);
        sf::Vector2i mapCoordsToPixel(const sf::Vector2f& point);
        void clear(sf::Color color);
        bool isOpen();
        bool pollEvent(sf::Event& event);
        void display();
        void draw(Drawable *sprite);
        void draw(Drawable *sprite, const sf::BlendMode& blendMode);
        void draw(sf::Drawable const& draw);
        void draw(sf::Drawable const& draw, const sf::BlendMode& blendMode);
        void close();
        void setView(View *view);
        void setActive(bool active);

        View *getView();
        sf::RenderWindow *getSFMLRenderWindow();
        // void setView(const &)

    private:
        View *_view = nullptr;
        sf::RenderWindow *_window;
        std::size_t _width;
        std::size_t _height;
};
