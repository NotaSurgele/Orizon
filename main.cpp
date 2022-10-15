#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Components/Sprite.hpp"
#include "RessourcesManager.hpp"
#include "Engine/Entity.hpp"
#include "Engine/Core.hpp"
#include "Engine/Entity.hpp"

int main(void)
{
	Entity e = Entity();

	sf::RenderWindow window(sf::VideoMode(600, 300) , "window");
	Core c = Core();

	Core::RManager().addRessource<sf::Texture>("player", "../assets/B_witch_attack.png");
	e.addComponent<Sprite>()->setTexture(Core::RManager().getRessource<sf::Texture>("player"));
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
