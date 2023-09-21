#pragma once

#include <string>
#include "SFML/Audio.hpp"
#include "IComponent.hpp"
#include "Entity.hpp"

class OrizonMusic : public IComponent {
public:
   OrizonMusic(Entity *e) : _e(e) {};
    ~OrizonMusic() { delete _music; };


    bool load(const std::string& path);
    void play();
    void stop();
    void pause();

    void setVolume(const float& volume);
    void setLoop(bool loop);
    OrizonMusic *setMusic(sf::Music *music);
    bool getLoop();
    float getVolume();
    float getDuration();
    sf::Sound::SoundSource::Status getStatus();

    void destroy() override;

private:
    std::string _path;
    sf::Music *_music = nullptr;
    Entity *_e = nullptr;
};
