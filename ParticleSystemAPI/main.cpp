// main.cpp

#include "SFML\Graphics\RenderWindow.hpp"
#include "SFML\Window\Event.hpp"

#include "SFML\System\Vector2.hpp"

#include "ParticleSystem.h"
#include "SFML/Graphics/RectangleShape.hpp"

float deltaTime = 0.0f;
sf::Clock deltaClock;

sf::Vector2i targetPosition = sf::Vector2i(0, 0);

void updateDeltatime()
{
	deltaTime = deltaClock.restart().asSeconds();
}

int main(int argc, const char* argv[])
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(1280, 720), "ParticleSystemAPI - Demo");

	// Load a sprite to display
	// Start the game loop

	PS::ParticleSystem partSystem;
	PS::Particle fire = partSystem.CreateParticle();

	PS::Emitter fireplace = partSystem.CreateEmitter();

	partSystem.ParticleSetColor(fire, PS::Color(0.5f, 0.5f, 0.5f, 0.5f));
	partSystem.ParticleSetLifetime(fire, 5, 10);

	while (window.isOpen())
	{
		updateDeltatime();

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
					case sf::Keyboard::Escape:
						window.close();
					break;
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				}
				if (event.mouseButton.button == sf::Mouse::Button::Right)
				{
					
				}
			}
		}

		// Clear screen
		window.clear();

		// render particlesystem
		for (int i = 0; i < 16/*partSystem.ParticleCount()*/; i++)
		{
			sf::RectangleShape shape(sf::Vector2f(32.0f, 32.0f));

			shape.setOrigin(0, 0);
			shape.setPosition(i*16, (float)i*16);
			shape.setSize(sf::Vector2f(8, 8));

			window.draw(shape);
		}

		// Update the window
		window.display();
	}
	return EXIT_SUCCESS;
}