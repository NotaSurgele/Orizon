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
	Core::RManager().addRessource<sf::Texture>("player", "../assets/B_witch_attack.png");
	e.addComponent<Sprite>()->setTexture(Core::RManager().getRessource<sf::Texture>("player"));
	while (window.isOpen()) {
		c.time.update();
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		window.draw(e.getComponent<Sprite>());
		window.display();
		std::cout << c.time.getDeltaTime() << std::endl;
	}
	return 0;
}
