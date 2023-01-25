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

        void clear(sf::Color color);
        bool isOpen();
        bool pollEvent(sf::Event& event);
        void display();
        void draw(Drawable *sprite);
        void close();
        void setView(View *view);
        View *getView();
        // void setView(const &)

    private:
        View *_view = nullptr;
        sf::RenderWindow _window;
        std::size_t _width;
        std::size_t _height;
};
