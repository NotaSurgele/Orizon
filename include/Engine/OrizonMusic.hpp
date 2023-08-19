#pragma once

#include <string>
#include <SFML/Audio.hpp>

class OrizonMusic {
public:
   OrizonMusic() = default;
    ~OrizonMusic() = default;


    bool load(const std::string& path);
    void play();
    void stop();
    void pause();

    void setVolume(const float& volume);
    void setLoop(bool loop);

    bool getLoop();
    float getVolume();
    float getDuration();
    sf::Sound::SoundSource::Status getStatus();

private:
    std::string _path;
    sf::Music _music;
};