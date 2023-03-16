#include "Engine/Particle/Particles.hpp"

void Particles::setShape(const ParticleShape& shape, const float& size=10)
{
    for (auto& particle : _particles) {
        auto& img = particle.first;

        switch (shape) {
            case SQUARE:
                img = createSquare(size, sf::Color::White);
                break;
            default:
                break;
        }
        particle.first = img;
    }
}

void Particles::setColor(const sf::Color& color)
{
}

void Particles::setDensity(const std::size_t& density=1)
{
}

void Particles::setLifeTime(const float &lifeTime=1)
{
    _lifeTime = lifeTime;
}

void Particles::play(bool restart=false)
{
    float deltaTime = Time::deltaTime;

    if (_currentTime > _lifeTime) {
        if (restart)
            _currentTime = 0.0f;
        _isPlaying = false;
        return;
    }
    _currentTime += deltaTime;
    _isPlaying = true;
    for (auto& particle : _particles) {
        sf::Texture texture = sf::Texture();
        texture.loadFromImage(particle.first);
        particle.second.setTexture(texture);
        std::cout << "Should draw" << std::endl;
        DRAW(particle.second);
    }
}

sf::Image Particles::createSquare(const std::size_t& size, const sf::Color& color)
{
    sf::Image img = sf::Image();

    img.create(size, size, color);
    return img;
}
