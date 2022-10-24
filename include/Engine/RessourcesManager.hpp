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

        template<typename T>
        T& getRessource(std::string const &ressourceName)
        {
            return static_cast<T &>(_map[ressourceName]);
        }

    private:
        std::map<std::string, sf::Texture> _map;
};
