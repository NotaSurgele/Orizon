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

void Particles::setEmitter(const float &x=0, const float &y=0, const float &w=10, const float &h=10)
{
    _emitter.left = x;
    _emitter.top = y;
    _emitter.width = w;
    _emitter.height = h;

    for (auto& particle : _particles) {
        sf::Sprite sprite = particle.second;

    }
}

void Particles::setSeed(const unsigned int& seed)
{
    _seed = seed;
    srand(seed);
    for (auto& particle : _particles) {
        float x = RANDOMF(_emitter.left, _emitter.left + _emitter.width);
        float y = RANDOMF(_emitter.left, _emitter.left + _emitter.width);
        particle.second.setPosition(x, y);
    }
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
        DRAW(particle.second);
    }
}

bool Particles::isPlaying()
{
    return _isPlaying;
}

void Particles::restart()
{
    _currentTime = 0.0f;
}

sf::Image Particles::createSquare(const std::size_t& size, const sf::Color& color)
{
    sf::Image img = sf::Image();

    img.create(size, size, color);
    return img;
}
