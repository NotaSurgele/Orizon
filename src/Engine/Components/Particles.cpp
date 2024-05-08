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
            randomness = json["randomness"];
            lifeTime = json["life_time"];
            amount = json["amount"];
            rect.width = json["size"][0];
            rect.height = json["size"][1];
            texture = R_GET_RESSOURCE(sf::Texture, json["texture"]);
            scale = { json["texture_scale"][0], json["texture_scale"][1] };
        }
    },
    {
        "color", [&] (nlohmann::json& json) {
            color  = { json["rgba"][0], json["rgba"][1], json["rgba"][2], json["rgba"][3] };
        }
    },
    {
        "velocity", [&] (nlohmann::json& json) {
            velocity = { json["velocity"][0], json["velocity"][1] };
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

        blank.create(100, 100, color);
        texture = new sf::Texture;
        texture->loadFromImage(blank);
    }
    std::cout << "LOAD" << std::endl;
    for (std::size_t i = 0; i < amount; i++) {
        auto sprite = new Sprite(texture);
        auto pData = ParticleData();

        sprite->setPosition(0, 0);
        sprite->setColor(color);
        sprite->setScale(scale.x, scale.y);
        pData.sprite = sprite;
        pData.maxLifeTime = lifeTime;
        pData.currentLifeTime = 0.0f;
        _sprites.push_back(pData);
    }
}

void Particle::play(bool loop, const sf::Vector2f& entityPosition)
{
    //Base draw
    static bool set = false;
    static std::size_t deadParticle = 0;

    if (deadParticle >= amount) _hasFinished = true;
    if (!loop && _hasFinished) return;
    if (loop && _hasFinished) set = false;

    for (auto &pData : _sprites) {
        auto s = pData.sprite;

        if (!set) {
            s->setPosition(entityPosition + _offset);
            pData.isDead = false;
            pData.maxLifeTime = lifeTime;
            pData.currentLifeTime = 0.0f;
            deadParticle = 0;
            _hasFinished = false;
            continue;
        }
        // [TODO] move velocity into ParticleData for randomness, remove emitter behaviour
        pData.currentLifeTime += Time::deltaTime;

        if (pData.currentLifeTime >= pData.maxLifeTime) {
            pData.isDead = true;
            deadParticle += 1;
            continue;
        }
        auto fixedPosition = s->getPosition() + (velocity * Time::deltaTime);
        s->setPosition(fixedPosition);
        s->setColor(color);
        DRAW_BATCH(s);
    }
    set = true;
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
    for (auto& s : _sprites) {
        delete s.sprite;
    }
}
