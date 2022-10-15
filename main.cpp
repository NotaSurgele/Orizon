#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Components/Sprite.hpp"
#include "RessourcesManager.hpp"
#include "Engine/Entity.hpp"

int main(void)
{

	Entity e = Entity();
	sf::RenderWindow window(sf::VideoMode(600, 300) , "window");
	RessourcesManager r = RessourcesManager();

	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);
		window.display();
	}
	return 0;
}
