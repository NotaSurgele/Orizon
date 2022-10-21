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
	Core c = Core("hello world");

	R_ADD_RESSOURCE(sf::Texture, "player", "../assets/B_witch_attack.png");
	e.addComponent<Sprite>()->setTexture(R_GET_RESSOURCE(sf::Texture, "player"));
	while (c.isOpen()) {
		sf::Event event;
		Core::getTime().update();

		//Event call
		while (c.CoreEvent(event)) {
			if (event.type == sf::Event::Closed)
				c.CoreClose();
		}

		//Update call
		e.getComponent<Transform2D>()->position.x += 25 * Core::getDeltaTime();
		e.getComponent<Sprite>()->update();

		//Draw call
		c.CoreClear(sf::Color::Black);
		c.CoreDraw(e.getComponent<Sprite>());
		c.CoreDisplay();
	}
	return 0;
}
