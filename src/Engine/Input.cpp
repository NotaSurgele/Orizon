#include "Input.hpp"
#include <algorithm>
#include <SFML/System.hpp>
#include <fstream>

bool Input::isKeyPressed(std::string const& key)
{
    return std::find(___keyArray.begin(), ___keyArray.end(), key) != ___keyArray.end();
}

bool Input::isKeyReleased(std::string const& key)
{
    std::vector<std::string>::iterator it = std::find(___key_release.begin(),
                                                    ___key_release.end(), key);
    if (it != ___key_release.end()) {
        ___key_release.erase(it);
        return true;
    }
    return false;
}

bool Input::isKeyPressed()
{
    return ___key_down.size() > 0;
}

bool Input::isKeyDown(std::string const& key)
{
    std::vector<std::string>::iterator it = std::find(___key_down.begin(),
                                                    ___key_down.end(), key);

    if (it != ___key_down.end()) {
        ___key_down.erase(it);
        return true;
    }
    return false;
}

void Input::___push_key(sf::Keyboard::Key key)
{
    std::string key_pressed = _sfml_to_key[key];

    if (std::find(___keyArray.begin(), ___keyArray.end(), key_pressed) !=
                                                        ___keyArray.end())
        return;
    std::cout << "Push input nb : " << key_pressed << std::endl;
    Input::___keyArray.push_back(key_pressed);
    Input::___key_down.push_back(key_pressed);
}

void Input::___remove_key(sf::Keyboard::Key key)
{
    std::string to_remove = _sfml_to_key[key];
    std::vector<std::string>::iterator it = std::find(___keyArray.begin(),
                                                ___keyArray.end(), to_remove);

    if (___key_release.size() > 10)
        ___key_release.clear();
    if (it != ___keyArray.end()) {
        ___key_release.push_back(to_remove);
        std::cout << "remove input n: " << ___keyArray.at(std::distance(
                                    ___keyArray.begin(), it)) << std::endl;
        ___keyArray.erase(it);
    }
}

void Input::__add_action(std::string const& action, std::string const& input)
{
    PUSHED_ACTION(action);
    _action_map.insert(std::pair<std::string, std::string>(action, input));
}

bool Input::isActionPressed(std::string const& action)
{
    std::string key_related = "";

    if (_action_map[action].size() <= 0) {
        NO_ACTION_DEFINED(action);
        return false;
    }
    key_related = _action_map[action];
    return Input::isKeyPressed(key_related);
}

bool Input::isActionKeyReleased(std::string const& action)
{
    std::string key = _action_map[action];
    std::vector<std::string>::iterator it = std::find(___key_release.begin(),
                                                    ___key_release.end(), key);

    if (it != ___key_release.end()) {
        ___key_release.erase(it);
        return true;
    }
    return false;
}

bool Input::isActionKeyDown(std::string const& action)
{
    std::string key = _action_map[action];
    std::vector<std::string>::iterator it = std::find(___key_down.begin(),
                                                    ___key_down.end(), key);

    if (it != ___key_down.end()) {
        ___key_down.erase(it);
        return true;
    }
    return false;
}

void Input::loadFromFile(std::string const& file)
{
    std::string content = "";
    std::ifstream f(file);

    if (_action_map.size() != 0)
        _action_map.clear();
    if (!f.is_open()) {
        std::cerr << "[Input] Could not read from file " << file << std::endl;
        return;
    }
    while (std::getline(f, content)) {
        std::string action = content.substr(0, content.find("="));
        std::cout << "action = " << action.size() << std::endl;
        std::string input = content.substr(action.size() + 1, action.find("="));
        std::cout << "input = " << input << std::endl;
        __add_action(action, input);
    }
    f.close();
}
