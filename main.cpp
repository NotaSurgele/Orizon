#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Components/Sprite.hpp"
#include "RessourcesManager.hpp"
#include "Engine/Entity.hpp"
#include "Engine/Core.hpp"
#include "Engine/RenderWindow.hpp"
#include "Engine/Entity.hpp"
#include "Engine/Input.hpp"

int main(void)
{
	Entity e = Entity();
	Core c = Core("hello world");
	Input input = Input();

	R_ADD_RESSOURCE(sf::Texture, "player", "../assets/B_witch_attack.png");
	e.addComponent<Sprite>()->setTexture(R_GET_RESSOURCE(sf::Texture, "player"));
	while (c.isOpen()) {
		sf::Event event;
		Core::getTime().update();

		//Event call
		while (c.CoreEvent(event)) {
			if (event.type == sf::Event::Closed)
				c.CoreClose();
			if (event.type == sf::Event::KeyPressed)
				input.___push_key(event.key.code);
			if (event.type == sf::Event::KeyReleased)
				input.___remove_key(event.key.code);
		}
		std::cout << ": " << input.isKeyPressed("Down") << std::endl;
		//Update call
		e.getComponent<Transform2D>()->position.x += 1.5f * Core::getDeltaTime();
		e.getComponent<Sprite>()->update();

		//Draw call
		c.CoreClear(sf::Color::Black);
		c.CoreDraw(e.getComponent<Sprite>());
		c.CoreDisplay();
	}
	return 0;
}
