#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <functional>
#include "Core.hpp"

enum ParticleShape {
    SQUARE,
    RECTANGLE,
    NIL
};

class Particles {
public:
    Particles(const std::size_t& density) : _density(density)
    {
        srand(std::time(nullptr));
        for (std::size_t i = 0; i < density; i++) {
            sf::Image img = sf::Image();
            sf::Sprite sprite = sf::Sprite();
            float x = RANDOMF(_emitter.left, _emitter.left + _emitter.width);
            float y = RANDOMF(_emitter.left, _emitter.left + _emitter.width);
            std::cout << x << " " << y << std::endl;
            sprite.setPosition(x, y);
            _particles.push_back(std::pair<sf::Image, sf::Sprite>(img, sprite));
        }
        setShape(ParticleShape::SQUARE, 10);
    }

    Particles()
    {
        srand(_seed);
        for (std::size_t i = 0; i < _density; i++) {
            sf::Image img = sf::Image();
            sf::Sprite sprite = sf::Sprite();
            float x = RANDOMF(_emitter.left, _emitter.left + _emitter.width);
            float y = RANDOMF(_emitter.left, _emitter.left + _emitter.width);
            sprite.setPosition(x, y);
            _particles.push_back(std::pair<sf::Image, sf::Sprite>(img, sprite));
        }
        setShape(ParticleShape::SQUARE, 10);
    }

    ~Particles() = default;

    void setShape(const ParticleShape& shape, const float& size);

    void setColor(sf::Color const& color);

    void setDensity(const std::size_t& density);

    void setLifeTime(const float& lifeTime);

    void setEmitter(const float& x, const float& y, const float& w, const float& h);

    void play(bool repeat);

    void restart();

    void setSeed(const unsigned int& seed);

    bool isPlaying();

private:
    sf::Image createSquare(const std::size_t& size, const sf::Color& color);

private:
    sf::Image _particle;
    std::vector<std::pair<sf::Image, sf::Sprite>> _particles;
    float _lifeTime = 1.0f;
    std::size_t _density = 1;
    float _currentTime = 0.0f;
    bool _isPlaying = false;
    sf::FloatRect _emitter = {0, 0, 10, 10};
    unsigned int _seed = 0;
};