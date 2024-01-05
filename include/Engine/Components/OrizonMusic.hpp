#pragma once

#include <string>
#include "SFML/Audio.hpp"
#include "IComponent.hpp"
#include "Entity.hpp"

class OrizonMusic : public IComponent {
public:
   OrizonMusic(Entity *e, const std::string& name) : _e(e), _name(name) {};
    ~OrizonMusic() { delete _music; };


    bool load(const std::string& path);
    bool loadFromName(const std::string& name);

    void play();
    void stop();
    void pause();

    void setVolume(const float& volume);
    void setLoop(bool loop);
    OrizonMusic *setMusic(sf::Music *music);
    bool isLoop();
    float getVolume();
    float getDuration();
    sf::Sound::SoundSource::Status getStatus();
    std::string name() { return _name; }
    void setName(const std::string& name) { _name = name; }
    void destroy() override;

private:
    std::string _name;
    std::string _path;
    sf::Music *_music = nullptr;
    Entity *_e = nullptr;
};
