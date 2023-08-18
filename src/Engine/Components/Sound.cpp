#include "Sound.hpp"

Sound::Sound(Entity *e)
{
}

bool Sound::load(const std::string& filePath)
{
    if (!_buffer.loadFromFile(filePath))
        return false;
    _sound.setBuffer(_buffer);
    return true;
}

bool Sound::reload(const std::string& filePath)
{
    return load(filePath);
}

void Sound::pause()
{
    _sound.pause();
}

void Sound::play()
{
    _sound.play();
}

void Sound::stop()
{
    _sound.stop();
}

Sound *Sound::setLoop(bool loop)
{
    _sound.setLoop(loop);
    return this;
}

Sound *Sound::setVolume(const float& volume)
{
    _sound.setVolume(volume);
    return this;
}

Sound *Sound::setBuffer(const sf::SoundBuffer& buffer)
{
    _buffer = buffer;
    _sound.setBuffer(_buffer);
    return this;
}

float Sound::getVolume()
{
    return _sound.getVolume();
}

bool Sound::getLoop()
{
    return _sound.getLoop();
}

sf::Sound::Status Sound::getStatus()
{
    return _sound.getStatus();
}