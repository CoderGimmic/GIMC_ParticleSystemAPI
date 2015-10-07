// main.cpp

#include "SFML\Graphics\RenderWindow.hpp"
#include "SFML\Window\Event.hpp"

#include "SFML\System\Vector2.hpp"

#include "ParticleSystem.h"
#include "SFML/Graphics/RectangleShape.hpp"

#include <iostream>

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

	//system("pause");

	PS::ParticleSystem partSystem;
	PS::Particle fire = partSystem.CreateParticle();

	PS::Emitter fireplace = partSystem.CreateEmitter();

	partSystem.ParticleSetColor(fire, PS::Color(255, 255,128, 125));
	partSystem.ParticleSetLifetime(fire, 5.0f, 1.0f);

	partSystem.EmitterSetParticle(fireplace, fire);
	partSystem.EmitterSetLocation(fireplace, PS::Vector2(256.f, 256.f));
	partSystem.EmitterSetFrequency(fireplace, 0.25f);
	partSystem.EmitterSetShape(fireplace, PS::EmitterShape::POINT);

	while (window.isOpen())
	{
		updateDeltatime();

		partSystem.Update(deltaTime);

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
		for (unsigned i = 0; i < partSystem.GetParticleCount(); i++)
		{
			PS::Vector2 location = partSystem.GetParticleLocation(i);
			PS::Color color = partSystem.GetParticleColor(i);
			float size = partSystem.GetParticleSize(i);
			PS::Vector2 scale = partSystem.GetParticleScale(i);
			sf::RectangleShape shape(sf::Vector2f(32.0f, 32.0f));

			//shape.setOrigin(0, 0);
			shape.setPosition(location.X, location.Y);
			shape.setSize(sf::Vector2f(size, size));
			shape.setScale(sf::Vector2f(scale.X, scale.Y));
			shape.setFillColor(sf::Color(color.R, color.G, color.B, color.A));

			window.draw(shape);
		}

		// Update the window
		window.display();
	}
	return EXIT_SUCCESS;
}