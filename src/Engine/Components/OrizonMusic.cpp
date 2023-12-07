#include "OrizonMusic.hpp"
#include "Core.hpp"

bool OrizonMusic::load(const std::string& path)
{
    return _music->openFromFile(path);
}

bool OrizonMusic::loadFromName(const std::string &name)
{
    _music = R_GET_MUSIC(name);

    return _music != nullptr;
}

void OrizonMusic::setLoop(bool loop)
{
    _music->setLoop(loop);
}

void OrizonMusic::setVolume(const float& volume)
{
    _music->setVolume(volume);
}

OrizonMusic *OrizonMusic::setMusic(sf::Music *music)
{
    _music = music;
    return this;
}

sf::Sound::SoundSource::Status OrizonMusic::getStatus()
{
    return _music->getStatus();
}

bool OrizonMusic::isLoop()
{
    return _music->getLoop();
}

float OrizonMusic::getVolume()
{
    return _music->getVolume();
}

float OrizonMusic::getDuration()
{
    return _music->getDuration().asSeconds();
}

void OrizonMusic::play()
{
    _music->play();
}

void OrizonMusic::stop()
{
    _music->stop();
}

void OrizonMusic::pause()
{
    _music->pause();
}

void OrizonMusic::destroy()
{
    _music->stop();
    delete _music;
}
