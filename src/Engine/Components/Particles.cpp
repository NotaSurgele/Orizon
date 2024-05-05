#include "Particles.hpp"
#include "Utils.hpp"

//Emitter
void ParticlesEmitter::destroy()
{
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
    _json = Utils::readfileToJson(file);

    sf::Image blank;
    blank.create(100, 100, sf::Color::White);

    auto texture = new sf::Texture;
    texture->loadFromImage(blank);

    for (std::size_t i = 0; i < 10; i++) {
        auto sprite = new Sprite(texture);
        sprite->setPosition(0, 0);
        sprite->setColor(sf::Color::White);
        _sprites.push_back(sprite);
    }
}
