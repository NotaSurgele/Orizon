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
Particle::Particle(const std::string &file) : _behaviourMap({
    {
        "offset", [&] (nlohmann::json& json) {
            _offset.x = json["offset"][0];
            _offset.y = json["offset"][1];
        }
    },
    {
        "emitter", [&] (nlohmann::json& json) {
            std::cout << "emitter behaviour" << std::endl;
            randomness = json["randomness"];
            velocity = { json["velocity"][0], json["velocity"][1] };
            lifeTime = json["life_time"];
            amount = json["amount"];
            rect.width = json["size"][0];
            rect.height = json["size"][1];
            texture = R_GET_RESSOURCE(sf::Texture, json["texture"]);
            scale = { json["texture_scale"][0], json["texture_scale"][1] };
        }
    }
})
{
    _json = Utils::readfileToJson(file);

    try {
        for (auto& data : _json["data"]) {
            std::string dataName = data["data_name"];
            auto behaviour = _behaviourMap[dataName];

            behaviour(data);
        }
        load();

    } catch (std::exception& e) {
        std::cerr << "[PARTICLE] " << e.what() << std::endl;
    }
}

Particle::Particle()
{
    load();
}

void Particle::load()
{
    if (texture == nullptr) {
        sf::Image blank = sf::Image();

        blank.create(100, 100, sf::Color::White);
        texture = new sf::Texture;
        texture->loadFromImage(blank);
    }

    for (std::size_t i = 0; i < amount; i++) {
        auto sprite = new Sprite(texture);
        sprite->setPosition(0, 0);
        sprite->setColor(sf::Color::White);
        sprite->setScale(scale.x, scale.y);
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

bool Particle::hasFinished() const
{
    return _hasFinished;
}

void Particle::destroy()
{
    for (auto& s : _sprites)
        delete s;
}
