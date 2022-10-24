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

    Input::___keyArray.push_back(key_pressed);
}
