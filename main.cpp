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
#include "Game/App.hpp"

int main(void)
{
	App app("hello world");

	app.run();
	return 0;
}
