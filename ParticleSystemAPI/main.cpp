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

	std::cout << std::endl << "ParticleSystem: " <<
		sizeof(PS::ParticleSystem) / 1024;
	std::cout << std::endl << "ParticleOutput: " <<
		(sizeof(PS::ParticleSystem::ParticleOutput)*1000)/1024;

	std::cout << std::endl;

	/*system("pause");
	return 0;*/

	PS::ParticleSystem partSystem;
	PS::Particle fire = partSystem.CreateParticle();
	PS::Emitter fireplace = partSystem.CreateEmitter();

	PS::Particle fire2 = partSystem.CreateParticle();
	PS::Emitter fireplace2 = partSystem.CreateEmitter();

	partSystem.ParticleSetSize(fire, 32, 64, 5);
	partSystem.ParticleSetRotation(fire, 0.0f, 360.0f, 15.0f);
	partSystem.ParticleSetVelocity(fire, PS::Vector2(64, 16));
	partSystem.ParticleSetColor(fire, PS::Color(255, 0,0, 125)/*, PS::Color(255,0,0,0)*/);
	partSystem.ParticleSetLifetime(fire, 1.0f, 5.0f);

	partSystem.EmitterSetPoint(fireplace, PS::Vector2(256.f, 256.f));
	partSystem.EmitterSetParticle(fireplace, fire);
	partSystem.EmitterSetFrequency(fireplace, 0.25f);

	partSystem.ParticleSetSize(fire2, 32, 64, 5);
	partSystem.ParticleSetRotation(fire2, 0.0f, 360.0f, 15.0f);
	partSystem.ParticleSetVelocity(fire2, PS::Vector2(64, 16));
	partSystem.ParticleSetColor(fire2, PS::Color(0, 255, 0, 125)/*, PS::Color(255,0,0,0)*/);
	partSystem.ParticleSetLifetime(fire2, 1.0f, 5.0f);

	partSystem.EmitterSetPoint(fireplace2, PS::Vector2(512.f, 256.f));
	partSystem.EmitterSetParticle(fireplace2, fire2);
	partSystem.EmitterSetFrequency(fireplace2, 0.75f);

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
			{
				window.close();
				break;
			}
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
			PS::Output* particle = partSystem.GetParticle(i);
			PS::Vector2 location = particle->location;
			PS::Color color = particle->color;
			float size = particle->size;
			PS::Vector2 scale = particle->scale;
			sf::RectangleShape shape(sf::Vector2f(32.0f, 32.0f));

			shape.setPosition(location.X, location.Y);
			shape.setRotation(particle->rotation);
			shape.setSize(sf::Vector2f(size, size));
			shape.setOrigin(size / 2.0f, size / 2.0f);
			shape.setScale(sf::Vector2f(scale.X, scale.Y));
			shape.setFillColor(sf::Color(color.R, color.G, color.B, color.A));

			window.draw(shape);
		}

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
}