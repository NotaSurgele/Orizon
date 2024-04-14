#pragma once

#include "IComponent.hpp"
#include <SFML/Audio.hpp>

class Sound : public IComponent {
public:
    Sound(Entity *e, const std::string& name);
    ~Sound() = default;

    bool load(const std::string& filePath);
    bool loadFromName(const std::string& name);

    bool reload(const std::string& filePath);
    void pause();
    void play();
    void stop();

    void setName(const std::string& name)
    {
        _name = name;
    }

    Sound *setLoop(bool loop);
    Sound *setVolume(const float& volume);
    Sound *setBuffer(sf::SoundBuffer* buffer);

    std::string name() { return _name; }
    float getVolume();
    bool isLoop();
    sf::Sound::Status getStatus();

    void destroy() override {
    };

private:
    std::string _name;
    sf::SoundBuffer *_buffer;
    sf::Sound _sound;
};