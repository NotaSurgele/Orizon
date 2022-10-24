#include "Input.hpp"
#include <algorithm>
#include <SFML/System.hpp>
#include <fstream>

bool Input::isKeyPressed(std::string const& key)
{
    return std::find(___keyArray.begin(), ___keyArray.end(), key) != ___keyArray.end();
}

void Input::___push_key(sf::Keyboard::Key key)
{
    std::string key_pressed = _sfml_to_key[key];

    if (std::find(___keyArray.begin(), ___keyArray.end(), key_pressed) != ___keyArray.end())
        return;
    std::cout << "Push input nb : " << key_pressed << std::endl;
    Input::___keyArray.push_back(key_pressed);
}

void Input::___remove_key(sf::Keyboard::Key key)
{
    std::string to_remove = _sfml_to_key[key];
    std::vector<std::string>::iterator it = std::find(___keyArray.begin(), ___keyArray.end(), to_remove);

    if (it != ___keyArray.end()) {
        std::cout << "remove input n: " << ___keyArray.at(std::distance(___keyArray.begin(), it)) << std::endl;
        ___keyArray.erase(it);
    }
}

void Input::__add_action(std::string const& action, std::string const& input)
{
    std::cout << "[Input] push action " << action << " with the input " << input << std::endl;
    _action_map.insert(std::pair<std::string, std::string>(action, input));
}

bool Input::isActionPressed(std::string const& action)
{
    std::string key_related = "";

    if (!_action_map[action].size() <= 0) {
        std::cout << "[Input] there is no action " << action << " dedfined !" << std::endl;
        return false;
    }
    key_related = _action_map[action];
    return Input::isKeyPressed(key_related);
}

void Input::loadFromFile(std::string const& file)
{
    std::string content = "";
    std::ifstream f(file);

    if (!f.is_open()) {
        std::cerr << "Could not read from file " << file << std::endl;
        return;
    }
    while (std::getline(f, content)) {
        std::string action = content.substr(0, content.find("="));
        std::string input = content.substr(action.size() + 1, content.find("="));
        __add_action(action, input);
    }
    f.close();
}

