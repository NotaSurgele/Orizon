#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "Engine/Components/Sprite.hpp"

#define DRAW_NULL_ERROR "Error [Window] draw sprite method: pointer is null 0x0" \
                        " cannot draw"

class RenderWindow {
    public:
        RenderWindow(std::string const& windowName,
                std::size_t width = 800,
                std::size_t height = 600);
        ~RenderWindow() = default;

        void clear(sf::Color color);
        bool isOpen();
        bool pollEvent(sf::Event& event);
        void display();
        void draw(Sprite *sprite);
        void close();

    private:
        sf::RenderWindow _window;
        std::size_t _width;
        std::size_t _height;
};