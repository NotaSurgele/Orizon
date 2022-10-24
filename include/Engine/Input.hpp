#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <iostream>

class Input {
public:
    Input() = default;
    ~Input() = default;

    enum Key {
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R,
        S, T, U, V, W, X, Y, Z, Num0, Num1, Num2, Num3, Num4,
        Num5, Num6, Num7, Num8, Num9,
        Escape, LControl, LShift, LAlt, LSystem, RControl, RShift,
        RAlt, RSystem, Menu, LBracket, RBracket, Semicolon, Comma,
        Period, Quote, Slash,  Backslash, Tilde,
        Equal, Hyphen, Space, Enter,
        Backspace, Tab, PageUp, PageDown,
        End, Home, Insert, Delete,
        Add, Subtract, Multiply, Divide,
        Left, Right, Up, Down,
        Numpad0, Numpad1, Numpad2, Numpad3,
        Numpad4, Numpad5, Numpad6, Numpad7,
        Numpad8, Numpad9, F1, F2,
        F3, F4, F5, F6,
        F7, F8, F9, F10,
        F11, F12, F13, F14,
    };
    static inline std::vector<Key> ___keyArray;
    static inline bool isKeyPressed(Key key);
    void ___push_key(sf::Keyboard::Key key);
    void ___remove_key(sf::Keyboard::Key key);


private:
    std::unordered_map<sf::Keyboard::Key, Key> _key_map = {
        {sf::Keyboard::Key::A, Key::A}, {sf::Keyboard::Key::B, Key::B}, {sf::Keyboard::Key::C, Key::C},
        {sf::Keyboard::Key::D, Key::D},{sf::Keyboard::Key::E, Key::E},{sf::Keyboard::Key::F, Key::F},
        {sf::Keyboard::Key::G, Key::G}, {sf::Keyboard::Key::H, Key::H}, {sf::Keyboard::Key::I, Key::I},
        {sf::Keyboard::Key::J, Key::J}, {sf::Keyboard::Key::K, Key::K}, {sf::Keyboard::Key::L, Key::L},
        {sf::Keyboard::Key::M, Key::M}, {sf::Keyboard::Key::N, Key::N},{sf::Keyboard::Key::O, Key::O},
        {sf::Keyboard::Key::P, Key::P}, {sf::Keyboard::Key::Q, Key::Q}, {sf::Keyboard::Key::R, Key::R},
        {sf::Keyboard::Key::S, Key::S}, {sf::Keyboard::Key::T, Key::T}, {sf::Keyboard::Key::U, Key::U},
        {sf::Keyboard::Key::V, Key::V}, {sf::Keyboard::Key::W, Key::W}, {sf::Keyboard::Key::X, Key::X},
        {sf::Keyboard::Key::Y, Key::Y}, {sf::Keyboard::Key::Z, Key::Z}, {sf::Keyboard::Key::Num0, Key::Num0},
        {sf::Keyboard::Key::Num0, Key::Num0}, {sf::Keyboard::Key::Num1, Key::Num1}, {sf::Keyboard::Key::Num2, Key::Num2},
        {sf::Keyboard::Key::Num3, Key::Num3}, {sf::Keyboard::Key::Num4, Key::Num4}, {sf::Keyboard::Key::Num5, Key::Num5},
        {sf::Keyboard::Key::Num6, Key::Num6}, {sf::Keyboard::Key::Num7, Key::Num7}, {sf::Keyboard::Key::Num8, Key::Num8},
        {sf::Keyboard::Key::Num9, Key::Num9}, {sf::Keyboard::Key::Escape, Key::Escape}, {sf::Keyboard::Key::LControl, Key::LControl},
        {sf::Keyboard::Key::LShift, Key::LShift}, {sf::Keyboard::Key::LAlt, Key::LAlt}, {sf::Keyboard::Key::LSystem, Key::LSystem},
        {sf::Keyboard::Key::RControl, Key::RControl}, {sf::Keyboard::Key::RShift, Key::RShift}, {sf::Keyboard::Key::RAlt, Key::RAlt},
        {sf::Keyboard::Key::RSystem, Key::RSystem}, {sf::Keyboard::Key::Menu, Key::Menu}, {sf::Keyboard::Key::LBracket, Key::LBracket},
        {sf::Keyboard::Key::RBracket, Key::RBracket}, {sf::Keyboard::Key::Semicolon, Key::Semicolon}, {sf::Keyboard::Key::Comma, Key::Comma},
        {sf::Keyboard::Key::Period, Key::Period}, {sf::Keyboard::Key::Quote, Key::Quote}, {sf::Keyboard::Key::Slash, Key::Slash},
        {sf::Keyboard::Key::Backslash, Key::Backslash}, {sf::Keyboard::Key::Tilde, Key::Tilde}, {sf::Keyboard::Key::Equal, Key::Equal},
        {sf::Keyboard::Key::Hyphen, Key::Hyphen}, {sf::Keyboard::Key::Space, Key::Space}, {sf::Keyboard::Key::Enter, Key::Enter},
        {sf::Keyboard::Key::Backspace, Key::Backspace}, {sf::Keyboard::Key::Tab, Key::Tab}, {sf::Keyboard::Key::PageUp, Key::PageUp},
        {sf::Keyboard::Key::PageDown, Key::PageDown}, {sf::Keyboard::Key::End, Key::End}, {sf::Keyboard::Key::Home, Key::Home},
        {sf::Keyboard::Key::Insert, Key::Insert}, {sf::Keyboard::Key::Delete, Key::Delete}, {sf::Keyboard::Key::Add, Key::Add},
        {sf::Keyboard::Key::Subtract, Key::Subtract}, {sf::Keyboard::Key::Multiply, Key::Multiply}, {sf::Keyboard::Key::Divide, Key::Divide},
        {sf::Keyboard::Key::Left, Key::Left}, {sf::Keyboard::Key::Right, Key::Right}, {sf::Keyboard::Key::Up, Key::Up},
        {sf::Keyboard::Key::Down, Key::Down}, {sf::Keyboard::Key::Numpad0, Key::Numpad0}, {sf::Keyboard::Key::Numpad1, Key::Numpad1},
        {sf::Keyboard::Key::Numpad2, Key::Numpad2}, {sf::Keyboard::Key::Numpad3, Key::Numpad3}, {sf::Keyboard::Key::Numpad4, Key::Numpad4},
        {sf::Keyboard::Key::Numpad5, Key::Numpad5}, {sf::Keyboard::Key::Numpad6, Key::Numpad6}, {sf::Keyboard::Key::Numpad7, Key::Numpad7},
        {sf::Keyboard::Key::Numpad8, Key::Numpad8}, {sf::Keyboard::Key::Numpad9, Key::Numpad9}, {sf::Keyboard::Key::F1, Key::F1},
        {sf::Keyboard::Key::F2, Key::F2}, {sf::Keyboard::Key::F3, Key::F3}, {sf::Keyboard::Key::F4, Key::F4},
        {sf::Keyboard::Key::F5, Key::F5}, {sf::Keyboard::Key::F6, Key::F6}, {sf::Keyboard::Key::F7, Key::F7},
        {sf::Keyboard::Key::F8, Key::F8}, {sf::Keyboard::Key::F9, Key::F9}, {sf::Keyboard::Key::F10, Key::F10},
        {sf::Keyboard::Key::F11, Key::F11}, {sf::Keyboard::Key::F12, Key::F12}, {sf::Keyboard::Key::F13, Key::F13},
        {sf::Keyboard::Key::F14, Key::F14}

    };
};
