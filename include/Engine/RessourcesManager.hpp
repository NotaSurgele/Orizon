#pragma once
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include "Sound.hpp"
#include"OrizonMusic.hpp"
#include "Utils.hpp"

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
            sf::Music *music = new sf::Music();

            music->openFromFile(filePath);
            // music->load(filePath);
            _musicMap.insert(std::pair<std::string, sf::Music *>(ressourceName, music));
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

        RessourcesManager& loadScript(const std::string& filepath)
        {
            std::string content = Utils::readFile(filepath, true);

            _scriptMap.insert(std::pair<std::string, std::string>(filepath, content));
            std::cout << "[RESOURCES MANAGER] New Resource Script " << filepath << " !" << std::endl;
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

        sf::Music *getMusic(std::string const& ressourceName)
        {
            return _musicMap[ressourceName];
        }

        std::string& getScript(const std::string& path)
        {
            return _scriptMap[path];
        }

        void destroy()
        {

            for (auto &music : _musicMap) {
                music.second->stop();
                delete music.second;
            }
        }

    private:
        std::map<std::string, sf::Texture> _map;
        std::map<std::string, sf::SoundBuffer> _soundMap;
        std::map<std::string, sf::Music *> _musicMap;
        std::map<std::string, std::string> _scriptMap;
};
