#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

void test(void);

int main(void)
{
	sf::RenderWindow window(sf::VideoMode(600, 300) , "window");

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
