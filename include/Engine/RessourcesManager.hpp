#pragma once
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include "Sound.hpp"
#include"OrizonMusic.hpp"

class RessourcesManager {
    public:
        RessourcesManager() = default;
        ~RessourcesManager() = default;

        template<typename T>
        RessourcesManager& addRessource(std::string const& ressourceName,
                                std::string const& filePath)
        {
            T ressource = T();

            ressource.loadFromFile(filePath);
            if constexpr (std::is_same_v<T, sf::SoundBuffer>) {
                _soundMap.insert(std::pair<std::string, sf::SoundBuffer>(ressourceName, ressource));
                return *this;
            } else
                _map.insert(std::pair<std::string, T>(ressourceName, ressource));
            return *this;
        }

        RessourcesManager& loadMusic(const std::string& ressourceName,
                                    const std::string& filePath)
        {
            OrizonMusic *music = new OrizonMusic();

            music->load(filePath);
            _musicMap.insert(std::pair<std::string, OrizonMusic *>(ressourceName, music));
            return *this;
        }

        RessourcesManager& loadTileFromSpriteSheet(std::string const& tilename,
                std::string const& filepath, int x, int y, int w, int h)
        {
            sf::Texture tile = sf::Texture();

            tile.loadFromFile(filepath, sf::IntRect{x, y, w, h});
            _map.insert(std::pair<std::string, sf::Texture>(tilename, tile));
            return *this;
        }

        template<typename T>
        T& getRessource(std::string const &ressourceName)
        {
            if constexpr (std::is_same_v<T, sf::SoundBuffer>)
                return static_cast<sf::SoundBuffer &>(_soundMap[ressourceName]);
            else
                return static_cast<T &>(_map[ressourceName]);
        }

        OrizonMusic *getMusic(std::string const& ressourceName)
        {
            return _musicMap[ressourceName];
        }

    private:
        std::map<std::string, sf::Texture> _map;
        std::map<std::string, sf::SoundBuffer> _soundMap;
        std::map<std::string, OrizonMusic *> _musicMap;
};
