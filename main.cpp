#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Components/Sprite.hpp"
#include "RessourcesManager.hpp"
#include "Engine/Entity.hpp"

#include "Engine/Entity.hpp"

int main(void)
{

	Entity e = Entity();

	sf::RenderWindow window(sf::VideoMode(600, 300) , "window");
	RessourcesManager r = RessourcesManager();

	r.addRessource<sf::Texture>("player", "../assets/B_witch_attack.png");
	e.addComponent<Sprite>()->setTexture(r.getRessource<sf::Texture>("player"));
	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		window.draw(e.getComponent<Sprite>()->getSprite());
		window.display();
	}
	return 0;
}
