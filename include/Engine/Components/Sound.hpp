#pragma once

#include "IComponent.hpp"
#include "Entity.hpp"
#include <SFML/Audio.hpp>

class Sound : public IComponent {
public:
    Sound(Entity *e);
    ~Sound() = default;

    bool load(const std::string& filePath);

    bool reload(const std::string& filePath);
    void pause();
    void play();
    void stop();

    Sound *setLoop(bool loop);
    Sound *setVolume(const float& volume);
    Sound *setBuffer(const sf::SoundBuffer& buffer);

    float getVolume();
    bool getLoop();
    sf::Sound::Status getStatus();

    void destroy() override {
    };

private:
    sf::SoundBuffer _buffer;
    sf::Sound _sound;
};