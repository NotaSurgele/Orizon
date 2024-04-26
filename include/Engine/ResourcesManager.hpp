#pragma once
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include "Sound.hpp"
#include"OrizonMusic.hpp"
#include "Utils.hpp"

class ResourcesManager {
    public:
        ResourcesManager() = default;
        ~ResourcesManager() = default;

        template<typename T>
        ResourcesManager& addRessource(std::string const& ressourceName,
                                       std::string const& filePath)
        {
            T *ressource = new T();

            ressource->loadFromFile(filePath);
            _pathMap.insert(std::pair<std::string, std::string>(ressourceName, filePath));
            if constexpr (std::is_same_v<T, sf::SoundBuffer>) {
                _soundMap.insert(std::pair<std::string, sf::SoundBuffer *>(ressourceName, ressource));
                return *this;
            } else
                _map.insert(std::pair<std::string, T *>(ressourceName, ressource));
            return *this;
        }

        ResourcesManager& loadShader(const std::string& resourceName,
                                     const std::string& vertex,
                                     const std::string& fragment)
        {
            auto shader = new Shader;

            if (!Shader::isAvailable())
               return *this;
            shader->loadFromFile(vertex, fragment);
            _shaderMap.insert(std::pair<std::string, Shader *>(resourceName, shader));
            return *this;
        }

        ResourcesManager& loadMusic(const std::string& ressourceName,
                                    const std::string& filePath)
        {
            sf::Music *music = new sf::Music();

            music->openFromFile(filePath);
            // music->load(filePath);
            _pathMap.insert(std::pair<std::string, std::string>(ressourceName, filePath));
            _musicMap.insert(std::pair<std::string, sf::Music *>(ressourceName, music));
            return *this;
        }

        ResourcesManager& loadTileFromSpriteSheet(std::string const& tilename,
                                                  std::string const& filepath, int x, int y, int w, int h)
        {
            sf::Texture *tile = new sf::Texture();
            if (!tile->loadFromFile(filepath,
               sf::IntRect{x, y, w, h})) {
                std::cerr << "[ResourceManager] ERROR cannot load Tile " << filepath << " at position "
                    << x << " " << y << std::endl;
            }
            _pathMap.insert(std::pair<std::string, std::string>(tilename, filepath));
            _map.insert(std::pair<std::string, sf::Texture *>(tilename, tile));
            return *this;
        }

        ResourcesManager& loadScript(const std::string& filepath)
        {
            if (_scriptMap.contains(filepath)) return *this;
            std::string content = Utils::readFile(filepath, true);

            _scriptMap.insert(std::pair<std::string, std::string>(filepath, content));
            std::cout << "[RESOURCES MANAGER] New Resource Script " << filepath << " !" << std::endl;
            return *this;
        }

        ResourcesManager& addTag(const std::string& tag)
        {
            _tagArray[tag] = 0;
            return *this;
        }

        std::size_t incrementTag(const std::string& tag, std::string& old)
        {
            for (auto& it : _tagArray) {
                if (it.first.find(old) != std::string::npos) {
                    it.second--;
                    break;
                }
            }
            for (auto& it : _tagArray) {
                if (it.first.find(tag) != std::string::npos) {
                    return ++it.second;
                }
            }
            return -1;
        }

        std::string pathFromName(const std::string& name)
        {
            for (auto& it : _pathMap) {
                if (it.first == name) {
                    return it.second;
                }
            }
            return "";
        }

        template<typename T>
        T* getRessource(std::string const &ressourceName)
        {
            if constexpr (std::is_same_v<T, sf::SoundBuffer>)
                return static_cast<sf::SoundBuffer *>(_soundMap[ressourceName]);
            else if constexpr (std::is_same_v<T, sf::Texture>) {
                return static_cast<T *>(_map[ressourceName]);
            } else if constexpr (std::is_same_v<T, Shader>) {
                return static_cast<T *>(_shaderMap[ressourceName]);
            }
        }

        template<typename T>
        std::map<std::string, T *>& getRessources()
        {
            if constexpr (std::is_same_v<T, sf::SoundBuffer>)
                return _soundMap;
            else if constexpr (std::is_same_v<T, sf::Texture>) {
                return _map;
            } else if constexpr (std::is_same_v<T, Shader>) {
                return _shaderMap;
            }
        }

        sf::Music *getMusic(std::string const& ressourceName)
        {
            return _musicMap[ressourceName];
        }

        std::map<std::string, sf::Music *>& getMusics()
        {
            return _musicMap;
        }

        std::string& getScript(const std::string& path)
        {
            return _scriptMap[path];
        }

        std::map<std::string, std::string>& getScripts()
        {
            return _scriptMap;
        }

        std::map<std::string, std::size_t> getTags()
        {
            return _tagArray;
        }

        std::map<std::string, Shader *>& getShader()
        {
            return _shaderMap;
        }

        std::string textureToName(const sf::Texture *texture)
        {
            for (auto& it : _map) {
                bool equal = true;
                sf::Image image1 = it.second->copyToImage();
                sf::Image image2 = texture->copyToImage();

                // Compare the size of the images
                if (image1.getSize() != image2.getSize()) {
                    continue;
                }

                // Compare the pixel data
                for (unsigned int x = 0; x < image1.getSize().x; ++x) {
                    for (unsigned int y = 0; y < image1.getSize().y; ++y) {
                        if (image1.getPixel(x, y) != image2.getPixel(x, y)) {
                            equal = false;
                            break;
                        }
                    }
                    if (!equal) break;
                }
                if (equal) return it.first;
            }
            return "";
        }

        void destroy()
        {

            for (auto &music : _musicMap) {
                music.second->stop();
                delete music.second;
            }
        }

    private:
        std::unordered_map<std::string, std::string> _pathMap;
        std::map<std::string, Shader *> _shaderMap;
        std::map<std::string, sf::Texture *> _map;
        std::map<std::string, sf::SoundBuffer *> _soundMap;
        std::map<std::string, sf::Music *> _musicMap;
        std::map<std::string, std::string> _scriptMap;
        std::map<std::string, std::size_t> _tagArray;
};
