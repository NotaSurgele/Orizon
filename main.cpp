#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Components/Sprite.hpp"
#include "RessourcesManager.hpp"
#include "Engine/Entity.hpp"
#include "Engine/Core.hpp"
#include "Engine/RenderWindow.hpp"
#include "Engine/Entity.hpp"

int main(void)
{
	Entity e = Entity();
	Core c = Core();

	RenderWindow window("hello world");
	R_ADD_RESSOURCE(sf::Texture, "player", "../assets/B_witch_attack.png");
	e.addComponent<Sprite>()->setTexture(R_GET_RESSOURCE(sf::Texture, "player"));
	while (window.isOpen()) {
		sf::Event event;

		//Event call
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//Update call
		c._time.update();
		e.getComponent<Transform2D>()->position.x += 25 * Core::getDeltaTime();
		e.getComponent<Sprite>()->update();

		//Draw call
		window.clear(sf::Color::Black);
		window.draw(e.getComponent<Sprite>());
		window.display();
	}
	return 0;
}
