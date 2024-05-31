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
        "fade_out", [&] (ParticleData& pData, nlohmann::json& json) {
            auto fadeOutData = ParticleData::FadeOutData();

            fadeOutData.end = false;
            fadeOutData.timer = Timer{ json["start_time"] };
            fadeOutData.speed = json["speed"];
            fadeOutData.to = { json["to"][0],  json["to"][1], json["to"][2], json["to"][3] };
            pData.fadeOutData = fadeOutData;
        }

    },
    {
        "fade_in", [&] (ParticleData& pData, nlohmann::json& json) {
            auto fadeInData = ParticleData::FadeInData();

            fadeInData.end = false;
            fadeInData.speed = json["speed"];
            fadeInData.to = { json["to"][0],  json["to"][1], json["to"][2], json["to"][3] };
            pData.fadeInData = fadeInData;
        }
    }
})
{
    _json = Utils::readfileToJson(file);
    path = file;
    try {
        initData(_json["emitter"]);

        load();
        for (auto& pData : _deadParticle) {
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

void Particle::load(const std::size_t& nb)
{
    try {
        for (std::size_t i = 0; i < nb; i++) {
            auto pData = ParticleData();
            auto sprite = new Sprite{texture};

            sprite->setPosition(0, 0);
            sprite->setColor(color);
            sprite->setScale(scale.x, scale.y);

            pData.spriteData.sprite = sprite;
            pData.lifeTimer.set(lifeTime);
            pData.id = amount + i;
            _deadParticle.push_back(pData);
        }
        for (auto &pData: _deadParticle) {
            for (auto &data: _json["data"]) {
                std::string dataName = data["data_name"];
                auto behaviour = _behaviourMap[dataName];

                behaviour(pData, data);
            }
        }
    } catch (std::exception& e) {
        throw "[PARTICLE] failed loading n: " + std::to_string(nb) + " particles cause " + e.what();
    }
}

void Particle::reload()
{
    destroy();
    load();
    for (auto& pData : _deadParticle) {
        for (auto &data: _json["data"]) {
            std::string dataName = data["data_name"];
            auto behaviour = _behaviourMap[dataName];

            behaviour(pData, data);
        }
    }
}

sf::RectangleShape& Particle::getEmitterShape()
{
    return _shape;
}

void Particle::initData(nlohmann::json& json)
{
    lifeTime = json["life_time"];
    amount = json["amount"];
    amountMin = json["amount_min"];
    amountMax = json["amount_max"];
    rect = { json["offset"][0], json["offset"][1],
             json["size"][0], json["size"][1] };
    delay = json["delay"];

    // Set shape data
    _shape = sf::RectangleShape();
    _shape.setPosition(rect.left, rect.top);
    _shape.setSize(rect.getSize());
    _shape.setFillColor(sf::Color::White);
    _shape.setOutlineColor(sf::Color::Green);
    _shape.setOutlineThickness(4.0f);
    //

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
        pData.id = i;
        _deadParticle.push_back(pData);
    }
}

void Particle::resetSpriteData(ParticleData::SpriteData &spriteData, const sf::Vector2f& ePosition)
{
    auto& s = spriteData.sprite;

    s->setPosition(ePosition + (rect.getPosition() + (rect.getSize() / 2.0f)));
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

void Particle::resetFadeOut(std::optional<ParticleData::FadeOutData>& fadeOut, ParticleData::SpriteData& spriteData)
{
    if (!fadeOut.has_value()) return;
    fadeOut->end = false;
    spriteData.currentColor.a = spriteData.color.a;
    fadeOut->timer.reset();
}

void Particle::fadeSystem(ParticleData::SpriteData &spriteData, std::optional<ParticleData::FadeInData> &fadeIn,
                          std::optional<ParticleData::FadeOutData> &fadeOut)
{
    if (fadeIn.has_value() && !fadeIn->end)
        fadeInSystem(spriteData, fadeIn);

    if (!fadeOut.has_value()) return;
    if (fadeOut->timer.ended()) {
        fadeOutSystem(spriteData, fadeOut);
        return;
    }
    fadeOut->timer.update();
}

void Particle::fadeOutSystem(ParticleData::SpriteData &pData, std::optional<ParticleData::FadeOutData> &fadeOut)
{
    if (!fadeOut.has_value() || fadeOut->end) return;
    auto& a = pData.currentColor.a;
    auto& r = pData.currentColor.r;
    auto& g = pData.currentColor.g;
    auto& b = pData.currentColor.b;
    auto& toA = fadeOut->to.a;
    auto& toR = fadeOut->to.r;
    auto& toG = fadeOut->to.g;
    auto& toB = fadeOut->to.b;
    auto aFloat = static_cast<float>(a);
    auto rFloat = static_cast<float>(r);
    auto gFloat = static_cast<float>(g);
    auto bFloat = static_cast<float>(b);

    if (a <= toA && r <= toR && g <= toG && b <= toB) {
        fadeOut->end = true;
        return;
    }

    float speed = fadeOut->speed * Time::deltaTime;

    aFloat -= speed;
    rFloat -= speed;
    gFloat -= speed;
    bFloat -= speed;

    aFloat = std::max(aFloat, static_cast<float>(toA));
    rFloat = std::max(rFloat, static_cast<float>(toR));
    gFloat = std::max(gFloat, static_cast<float>(toG));
    bFloat = std::max(bFloat, static_cast<float>(toB));

    r = static_cast<sf::Uint8>(rFloat);
    g = static_cast<sf::Uint8>(gFloat);
    b = static_cast<sf::Uint8>(bFloat);
    a = static_cast<sf::Uint8>(aFloat);
}

void Particle::fadeInSystem(ParticleData::SpriteData &pData, std::optional<ParticleData::FadeInData> &fadeIn)
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

    float speed = fadeIn->speed * Time::deltaTime;

    aFloat += speed;
    rFloat += speed;
    gFloat += speed;
    bFloat += speed;

    aFloat = std::min(static_cast<float>(toA), aFloat);
    rFloat = std::min(static_cast<float>(toR), rFloat);
    gFloat = std::min(static_cast<float>(toG), gFloat);
    bFloat = std::min(static_cast<float>(toB), bFloat);

    r = static_cast<sf::Uint8>(rFloat);
    g = static_cast<sf::Uint8>(gFloat);
    b = static_cast<sf::Uint8>(bFloat);
    a = static_cast<sf::Uint8>(aFloat);
}

bool Particle::killParticle(ParticleData& pData, std::queue<std::size_t> &removeQueue)
{
    pData.isDead = true;
    pData.set = false;
    _deadParticle.push_back(pData);
    _removeQueue.push(pData.id);
    _totalDeadParticle += 1;
    return true;
}

void Particle::updateDelayTimer(Timer &delayTimer)
{

    if (delayTimer.to() != delay)
        delayTimer.set(delay);
    delayTimer.update();
    if (delayTimer.ended()) {
        if (!_deadParticle.empty()) {
            if (_particles.size() + 1 < amount && loop) {
                _particles.push_back(_deadParticle.front());
            }
            _deadParticle.pop_front();
        }
        delayTimer.reset();
    }
}

void Particle::play(const sf::Vector2f& entityPosition)
{
    //Base draw
    if (_deadParticle.size() >= amount) {
        _totalDeadParticle = 0;
        _hasFinished = true;
    }
    if (!loop && _hasFinished) return;

    //Handle delay
    updateDelayTimer(delayTimer);
    _shape.setPosition(entityPosition + rect.getPosition());
    _shape.setSize(rect.getSize());

    // Particle loop
    for (auto& pData : _particles) {
        auto& spriteData = pData.spriteData;
        auto& s = spriteData.sprite;
        auto& velocityData = pData.velocityData;
        auto& fadeIn = pData.fadeInData;
        auto& fadeOut = pData.fadeOutData;
        auto& lifeTimer = pData.lifeTimer;

        if (!pData.set) {
            resetSpriteData(spriteData, entityPosition);
            resetParticleData(pData);
            resetFadeOut(fadeOut, spriteData);
            resetFadeIn(fadeIn, spriteData);
            _hasFinished = false;
            continue;
        }

        if (lifeTimer.to() != lifeTime) lifeTimer.set(lifeTime);

        lifeTimer.update();
        if (lifeTimer.ended()) {
            killParticle(pData, _removeQueue );
            continue;
        }

        auto fixedPosition = s->getPosition();
        if (velocityData.has_value()) fixedPosition += velocityData->velocity * Time::deltaTime;
        fadeSystem(spriteData, fadeIn, fadeOut);
        s->setPosition(fixedPosition);
        s->setColor(spriteData.currentColor);
        DRAW_BATCH(s);
    }

    // remove dead particles from array
    while (!_removeQueue.empty()) {
        std::size_t current = _removeQueue.front();

        for (auto it = _particles.begin(); it != _particles.end(); it++) {
            auto& pData = it;

            if (pData->id == current) {
                _particles.erase(it);
                break;
            }
        }
        _removeQueue.pop();
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

std::list<ParticleData>& Particle::getParticlesData()
{
    return _particles;
}

void Particle::destroy()
{
    // clear queue
    while (!_removeQueue.empty())
        _removeQueue.pop();
    while (!_deadParticle.empty())
       _deadParticle.pop_front();

    for (auto& s : _particles) {
        delete s.spriteData.sprite;
    }
    _particles.clear();
}
