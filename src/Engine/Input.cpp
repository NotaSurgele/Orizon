#include "Input.hpp"
#include <algorithm>
#include <SFML/System.hpp>

bool Input::isKeyPressed(std::string key)
{
    return std::find(___keyArray.begin(), ___keyArray.end(), key) != ___keyArray.end();
}

void Input::___push_key(sf::Keyboard::Key key)
{
    std::string key_pressed = _sfml_to_key[key];

    std::cout << "Push input nb : " << key_pressed << std::endl;
    Input::___keyArray.push_back(key_pressed);
}

void Input::___remove_key(sf::Keyboard::Key key)
{
    std::string to_remove = _sfml_to_key[key];
    std::vector<std::string>::iterator it = std::find(___keyArray.begin(), ___keyArray.end(), to_remove);

    if (it != ___keyArray.end()) {
        std::cout << "find input n: " << ___keyArray.at(std::distance(___keyArray.begin(), it)) << std::endl;
        ___keyArray.erase(it);
    }
}

bool Input::isActionPressed(std::string action)
{
    std::string key_related = _action_map[action];

    return Input::isKeyPressed(key_related);
}

void Input::__add_action(std::string action, std::string name)
{
    _action_map.insert(std::pair<std::string, std::string>(action, name));
}
