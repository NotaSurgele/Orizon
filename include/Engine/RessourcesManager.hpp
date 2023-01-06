#pragma once
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include <memory>

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
            _map.insert(std::pair<std::string, T>(ressourceName, ressource));
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
            return static_cast<T &>(_map[ressourceName]);
        }

    private:
        std::map<std::string, sf::Texture> _map;
};
