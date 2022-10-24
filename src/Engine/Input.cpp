#include "Input.hpp"
#include <algorithm>
#include <SFML/System.hpp>

bool Input::isKeyPressed(Key key)
{
    return std::find(___keyArray.begin(), ___keyArray.end(), key) != ___keyArray.end();
}

void Input::___push_key(sf::Keyboard::Key key)
{
    Key key_pressed = _key_map[key];

    std::cout << "Push input nb : " << key_pressed << std::endl;
    Input::___keyArray.push_back(key_pressed);
}

void Input::___remove_key(sf::Keyboard::Key key)
{
    Key to_remove = _key_map[key];

    std::vector<Key>::iterator it = std::find(___keyArray.begin(), ___keyArray.end(), to_remove);
    if (it != ___keyArray.end()) {
        std::cout << "find input n: " << ___keyArray.at(std::distance(___keyArray.begin(), it)) << std::endl;
        ___keyArray.erase(it);
    }
}
