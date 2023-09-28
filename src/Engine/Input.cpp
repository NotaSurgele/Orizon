#include "Input.hpp"
#include "config.hpp"
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

bool Input::isButtonReleased(std::string const& key)
{
    std::vector<std::string>::iterator it = std::find(___button_release.begin(),
                                                      ___button_release.end(), key);
    if (it != ___button_release.end()) {
        ___button_release.erase(it);
        return true;
    }
    return false;
}

bool Input::isKeyPressed()
{
    return ___key_down.size() > 0;
}

bool Input::isButtonPressed()
{
    return ___button_down.size() > 0;
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

bool Input::isButtonDown(std::string const& key)
{
    std::vector<std::string>::iterator it = std::find(___button_down.begin(),
                                                      ___button_down.end(), key);

    if (it != ___button_down.end()) {
        ___button_down.erase(it);
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
    Input::___keyArray.push_back(key_pressed);
    Input::___key_down.push_back(key_pressed);
}

void Input::___push_button(sf::Mouse::Button button)
{
    std::string button_pressed = _sfml_to_button[button];

    if (std::find(___buttonArray.begin(), ___buttonArray.end(), button_pressed) !=
            ___buttonArray.end())
        return;
    Input::___buttonArray.push_back(button_pressed);
    Input::___button_down.push_back(button_pressed);
}

void Input::___remove_button(sf::Mouse::Button button)
{
    std::string to_remove = _sfml_to_button[button];
    std::vector<std::string>::iterator it = std::find(___buttonArray.begin(),
                                                      ___buttonArray.end(), to_remove);

    if (___button_release.size() > 10)
        ___button_release.clear();
    if (it != ___buttonArray.end()) {
        ___button_release.push_back(to_remove);
        ___buttonArray.erase(it);
    }
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
        ___keyArray.erase(it);
    }
}

void Input::__add_action(std::string const& action, std::string const& input)
{
    PUSHED_ACTION(action);
    _action_map.insert(std::pair<std::string, std::string>(action, input));
}

bool Input::isActionButtonPressed(std::string const& action)
{
    std::string button_related = "";

    if (_action_map[action].size() <= 0) {
        NO_ACTION_DEFINED(action);
        return false;
    }
    button_related = _action_map[action];
    return Input::isButtonPressed(button_related);
}

bool Input::isButtonPressed(const std::string &key)
{
    return std::find(___buttonArray.begin(), ___buttonArray.end(), key) != ___buttonArray.end();
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

bool Input::isActionButtonReleased(std::string const& action)
{
    std::string key = _action_map[action];
    std::vector<std::string>::iterator it = std::find(___button_release.begin(),
                                                      ___button_release.end(), key);

    if (it != ___button_release.end()) {
        ___button_release.erase(it);
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

bool Input::isActionButtonDown(std::string const& action)
{
    std::string key = _action_map[action];
    std::vector<std::string>::iterator it = std::find(___button_down.begin(),
                                                      ___button_down.end(), key);

    if (it != ___button_down.end()) {
        ___button_down.erase(it);
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
    if (!f.is_open() && DEBUG_MESSAGE) {
        std::cerr << "[Input] Could not read from file " << file << std::endl;
        return;
    }
    while (std::getline(f, content)) {
        std::string action = content.substr(0, content.find("="));
        std::string input = content.substr(action.size() + 1, action.find("="));
        __add_action(action, input);
    }
    f.close();
}
