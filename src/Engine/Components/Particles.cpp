#include "Particles.hpp"
#include "Utils.hpp"
#include "Core.hpp"

//Emitter
void ParticlesEmitter::destroy()
{
    for (auto& p : particles) {
        p.second.destroy();
    }
    particles.clear();
}

// Particle
std::unordered_map<std::string, std::function<void(nlohmann::json&)>> Particle::_behaviourMap = {
    {
        "animation", [](nlohmann::json& json) {
            std::cout << "Coucou a tous comment allez vous ? " << std::endl;
        }
    }
};

Particle::Particle(const std::string &file)
{
    std::cout << " coucou " << std::endl;
    _json = Utils::readfileToJson(file);
    std::cout << "hello world" << std::endl;
    sf::Image blank = sf::Image();
    blank.create(100, 100, sf::Color::White);

    auto texture = new sf::Texture;
    texture->loadFromImage(blank);

    for (std::size_t i = 0; i < amount; i++) {
        auto sprite = new Sprite(texture);
        sprite->setPosition(0, 0);
        sprite->setColor(sf::Color::White);
        _sprites.push_back(sprite);
    }
}

Particle::Particle()
{
    sf::Image blank = sf::Image();
    blank.create(100, 100, sf::Color::White);

    auto texture = new sf::Texture;
    texture->loadFromImage(blank);

    for (std::size_t i = 0; i < amount; i++) {
        auto sprite = new Sprite(texture);
        sprite->setPosition(0, 0);
        sprite->setColor(sf::Color::White);
        _sprites.push_back(sprite);
    }
}

void Particle::play(bool loop, const sf::Vector2f& entityPosition)
{
    //Base draw
    if (loop) reset();
    if (_hasFinished) return;
    for (auto &s : _sprites) {
        auto fixedPosition = entityPosition + _offset;

        s->setPosition(fixedPosition);
        DRAW_BATCH(s);
    }
    _hasFinished = true;
}

void Particle::reset()
{
    _hasFinished = false;
}

bool Particle::hasFinished()
{
    return _hasFinished;
}

void Particle::destroy()
{
    for (auto& s : _sprites)
        delete s;
}
