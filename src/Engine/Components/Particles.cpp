#include "Particles.hpp"
#include "Utils.hpp"
#include "Core.hpp"

#include <Time.hpp>

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
        "offset", [&] (ParticleData& pData, nlohmann::json& json) {
            _offset.x = json["offset"][0];
            _offset.y = json["offset"][1];
        }
    },
    {
        "sprite", [&] (ParticleData& pData, nlohmann::json& json) {
            auto& sprite = pData.spriteData;

            texture = R_GET_RESSOURCE(sf::Texture, json["texture"]);
            sprite.color  = { json["color"][0], json["color"][1], json["color"][2], json["color"][3] };
            sprite.scale = { json["scale"][0], json["scale"][1] };
            sprite.sprite->setTexture(texture, true);
            sprite.sprite->setScale(sprite.scale.x, sprite.scale.y);
        }
    },
    {
        "velocity", [&] (ParticleData& pData, nlohmann::json& json) {
            auto velocity = ParticleData::VelocityData();

            if (json.contains("random")) {
                velocity.random = true;
                velocity.minVelocity = { json["random"]["minVelocity"][0], json["random"]["minVelocity"][1] };
                velocity.maxVelocity = { json["random"]["maxVelocity"][0], json["random"]["maxVelocity"][1] };
            }
            velocity.velocity = { json["value"][0], json["value"][1] };
            pData.velocityData = velocity;
        }
    }
})
{
    _json = Utils::readfileToJson(file);

    try {
        initData(_json["emitter"]);

        load();
        for (auto& pData : _sprites) {
            for (auto &data: _json["data"]) {
                std::string dataName = data["data_name"];
                auto behaviour = _behaviourMap[dataName];

                behaviour(pData, data);
            }
        }

    } catch (std::exception& e) {
        std::cerr << "[PARTICLE] " << e.what() << std::endl;
    }
}

void Particle::initData(nlohmann::json& json)
{
    lifeTime = json["life_time"];
    amount = json["amount"];
    rect.width = json["size"][0];
    rect.height = json["size"][1];
    sf::Image blank = sf::Image();

    blank.create(100, 100, color);
    texture = new sf::Texture;
    texture->loadFromImage(blank);

    if (json.contains("seed")) seed = json["seed"];
}

Particle::Particle()
{
    load();
}

void Particle::load()
{
    for (std::size_t i = 0; i < amount; i++) {
        auto pData = ParticleData();
        auto sprite = new Sprite { texture };

        sprite->setPosition(0, 0);
        sprite->setColor(color);
        sprite->setScale(scale.x, scale.y);
        pData.maxLifeTime = lifeTime;
        pData.currentLifeTime = 0.0f;
        pData.spriteData.sprite = sprite;
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
        auto& s = pData.spriteData.sprite;

        if (!set) {
            s->setPosition(entityPosition + _offset);
            pData.isDead = false;
            pData.maxLifeTime = lifeTime;
            pData.currentLifeTime = 0.0f;
            deadParticle = 0;
            _hasFinished = false;
            continue;
        }
        pData.currentLifeTime += Time::deltaTime;

        if (pData.currentLifeTime >= pData.maxLifeTime) {
            pData.isDead = true;
            deadParticle += 1;
            continue;
        }
        auto fixedPosition = s->getPosition();

        if (pData.velocityData.has_value()) fixedPosition += (pData.velocityData->velocity * Time::deltaTime);
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
        delete s.spriteData.sprite;
    }
}
