// main.cpp

#include "SFML\Graphics\RenderWindow.hpp"
#include "SFML\Window\Event.hpp"

#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\Text.hpp"
#include "SFML\Graphics\Font.hpp"

#include "ParticleSystem.h"
#include "SFML/Graphics/RectangleShape.hpp"

#include <iostream>
#include <string>

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
		sizeof(PS::ParticleSystem) /* / 1024*/;
	std::cout << std::endl << "ParticleOutput: " <<
		(sizeof(PS::ParticleSystem::ParticleOutput)*1000)/1024;

	std::cout << std::endl;

	/*system("pause");
	return 0;*/

	sf::Font fnt;
	fnt.loadFromFile("../data/pixel.ttf");

	PS::ParticleSystem partSystem;
	PS::Particle fire = partSystem.CreateParticle();
	PS::Emitter fireplace = partSystem.CreateEmitter();

	PS::Particle fire2 = partSystem.CreateParticle();
	PS::Emitter fireplace2 = partSystem.CreateEmitter();

	partSystem.ParticleSetSize(fire, 4, 8, 5);
	partSystem.ParticleSetScale(fire, 1.0f, 0.5f);
	partSystem.ParticleSetRotation(fire, 0.0f, 360.0f, 15.0f,0,true);
	partSystem.ParticleSetSpeed(fire, 96, 128);
	partSystem.ParticleSetDirection(fire, 270 - 32, 270 + 32);
	//partSystem.ParticleSetVelocity(fire, PS::Vector2(64, 16));
	partSystem.ParticleSetColor(fire, PS::Color(255, 255,0, 255), PS::Color(255,0,0,0));
	partSystem.ParticleSetLifetime(fire, 5.0f, 5.0f);
	//partSystem.ParticleSetSpawnedParticle(fire, fire);

	partSystem.EmitterSetParticle(fireplace, fire, 20);
	partSystem.EmitterSetPoint(fireplace, PS::Vector2(256.f, 256.f));
	partSystem.EmitterSetRectangle(fireplace, PS::Vector2(256, 256), PS::Vector2(1080, 8));
	partSystem.EmitterSetFrequency(fireplace, 0.01f);
	
	partSystem.ParticleSetSize(fire2, 32, 64, 5);
	partSystem.ParticleSetRotation(fire2, 0.0f, 360.0f, 15.0f);
	partSystem.ParticleSetVelocity(fire2, PS::Vector2(64, 16));
	partSystem.ParticleSetColor(fire2, PS::Color(0, 255, 0, 125));
	partSystem.ParticleSetLifetime(fire2, 5.0f, 5.0f);

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
					partSystem.SpawnParticle(fire2, PS::Vector2(mousePos.x, mousePos.y));
				}
				if (event.mouseButton.button == sf::Mouse::Button::Right)
				{
					partSystem.ClearVisibleParticles();
				}
			}
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		partSystem.EmitterSetLocation(fireplace, PS::Vector2(mousePos.x, mousePos.y));

		window.setMouseCursorVisible(false);

		sf::Text partCountLabel;
		partCountLabel.setFont(fnt);
		partCountLabel.setColor(sf::Color::White);
		partCountLabel.setString("Particles: " + std::to_string(partSystem.GetParticleCount()));
		partCountLabel.setPosition(sf::Vector2f(32, 32));

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

		window.draw(partCountLabel);

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
}