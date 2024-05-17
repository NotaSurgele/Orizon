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
            sprite.currentColor = color;
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
    },
    {
        "fade_in", [&] (ParticleData& pData, nlohmann::json& json) {
            pData.fadeInData = (ParticleData::FadeInData) {
                .end = false,
                .speed = json["speed"],
                .to = { json["to"][0],  json["to"][1], json["to"][2], json["to"][3] }
            };
        }
    },
    {
        "fade_out", [&] (ParticleData& pData, nlohmann::json& json) {
            pData.fadeOutData = (ParticleData::FadeOutData) {
                .end = false,
                .speed = json["speed"],
                .to = { json["to"][0],  json["to"][1], json["to"][2], json["to"][3] }
            };
        }
    }
})
{
    _json = Utils::readfileToJson(file);

    try {
        initData(_json["emitter"]);

        load();
        for (auto& pData : _particles) {
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
    delay = json["delay"];

    sf::Image blank = sf::Image();
    blank.create(100, 100, color);
    texture = new sf::Texture;
    texture->loadFromImage(blank);

    delayTimer.set(delay);
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
        pData.spriteData.sprite = sprite;

        pData.lifeTimer.set(lifeTime);
        _particles.push_back(pData);
    }
}

void Particle::resetSpriteData(ParticleData::SpriteData &spriteData, const sf::Vector2f& ePosition)
{
    auto& s = spriteData.sprite;

    s->setPosition(ePosition + _offset);
    s->setColor(spriteData.color);
    spriteData.currentColor = spriteData.color;
}

void Particle::resetParticleData(ParticleData &pData) const
{
    pData.set = true;
    pData.isDead = false;
    pData.lifeTimer.reset();
}

void Particle::resetFadeIn(std::optional<ParticleData::FadeInData>& fadeIn, ParticleData::SpriteData& spriteData)
{
    if (!fadeIn.has_value()) return;
    fadeIn->end = false;
    spriteData.currentColor.a = 0;
}

void Particle::resetFadeOut(std::optional<ParticleData::FadeOutData>& fadeOut)
{
    if (!fadeOut.has_value()) return;
    fadeOut->end = false;
}

void Particle::fadeInSystem(ParticleData::SpriteData& pData, std::optional<ParticleData::FadeInData>& fadeIn)
{
    if (!fadeIn.has_value() || fadeIn->end) return;
    auto& a = pData.currentColor.a;
    auto& r = pData.currentColor.r;
    auto& g = pData.currentColor.g;
    auto& b = pData.currentColor.b;
    auto& toA = fadeIn->to.a;
    auto& toR = fadeIn->to.r;
    auto& toG = fadeIn->to.g;
    auto& toB = fadeIn->to.b;
    auto aFloat = static_cast<float>(a);
    auto rFloat = static_cast<float>(r);
    auto gFloat = static_cast<float>(g);
    auto bFloat = static_cast<float>(b);

    if (a >= toA && r >= toR && g >= toG && b >= toB) {
        fadeIn->end = true;
        return;
    }
    aFloat += (fadeIn->speed * Time::deltaTime);
    rFloat += (fadeIn->speed * Time::deltaTime);
    gFloat += (fadeIn->speed * Time::deltaTime);
    bFloat += (fadeIn->speed * Time::deltaTime);

    aFloat = std::min(aFloat, static_cast<float>(toA));
    rFloat = std::min(aFloat, static_cast<float>(toR));
    gFloat = std::min(aFloat, static_cast<float>(toG));
    bFloat = std::min(aFloat, static_cast<float>(toB));

    r = static_cast<sf::Uint8>(rFloat);
    g = static_cast<sf::Uint8>(gFloat);
    b = static_cast<sf::Uint8>(bFloat);
    a = static_cast<sf::Uint8>(aFloat);
}

void Particle::play(bool loop, const sf::Vector2f& entityPosition)
{
    //Base draw
    static std::size_t deadParticle = 0;
    static std::queue<std::size_t> removeQueue;

    std::size_t length = _particles.size();

    if (deadParticle >= amount) _hasFinished = true;
    if (!loop && _hasFinished) return;

    //Handle delay
    delayTimer.update();
    if (delayTimer.ended()) {
        if (!_deadParticle.empty()) {
            _particles.push_back(_deadParticle.front());
            _deadParticle.pop();
        }
        delayTimer.reset();
    }

    // Particle loop
    for (std::size_t i = 0; i < length; i++) {
        auto& pData = _particles[i];
        auto& spriteData = pData.spriteData;
        auto s = spriteData.sprite;
        auto& velocityData = pData.velocityData;
        auto& fadeIn = pData.fadeInData;
        auto& fadeOut = pData.fadeOutData;
        auto& lifeTimer = pData.lifeTimer;

        if (!pData.set) {
            resetSpriteData(spriteData, entityPosition);
            resetParticleData(pData);
            resetFadeIn(fadeIn, spriteData);
            resetFadeOut(fadeOut);
            deadParticle -= 1;
            _hasFinished = false;
            continue;
        }
        lifeTimer.update();

        if (lifeTimer.ended()) {
            pData.isDead = true;
            pData.set = false;
            deadParticle += 1;
            _deadParticle.push(pData);
            removeQueue.push(i);
            continue;
        }

        auto fixedPosition = s->getPosition();
        if (velocityData.has_value()) fixedPosition += velocityData->velocity * Time::deltaTime;
        fadeInSystem(spriteData, fadeIn);
        s->setPosition(fixedPosition);
        s->setColor(spriteData.currentColor);
        DRAW_BATCH(s);
    }

    // remove dead particles from array
    while (!removeQueue.empty()) {
        std::size_t& i = removeQueue.front();

        _particles.erase(_particles.begin() + i);
        removeQueue.pop();
    }
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
    for (auto& s : _particles) {
        delete s.spriteData.sprite;
    }
}
