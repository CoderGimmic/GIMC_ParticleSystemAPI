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

//#include <vld.h>

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

	// Start the game loop

	std::cout << std::endl << "ParticleSystem size: " <<
		sizeof(PS::ParticleSystem);

	std::cout << std::endl;

	/*system("pause");
	return 0;*/

	sf::Font fnt;
	fnt.loadFromFile("../data/pixel.ttf");

	PS::ParticleSystem partSystem;
	
	// FIRE
	/*-------------------------------------*/
	PS::Particle fire = partSystem.CreateParticle();
	partSystem.ParticleSetSize(fire, 1, 1, 5);
	partSystem.ParticleSetScale(fire, 1.0f, 0.1f);
	partSystem.ParticleSetRotation(fire, 0.0f, 360.0f, 0.0f, 0, false);
	partSystem.ParticleSetSpeed(fire, 96, 128, 96);
	partSystem.ParticleSetDirection(fire, 270 - 32, 270 + 32, 45);
	partSystem.ParticleSetColor(fire, PS::Color(255, 255,0, 255), PS::Color(255,0,0,0));
	partSystem.ParticleSetLifetime(fire, 2.0f, 2.0f);

	// Emitter #1
	PS::Emitter fireplace = partSystem.CreateEmitter(fire);
	partSystem.EmitterSetPoint(fireplace, PS::Vector2(256.f, 256.f));
	partSystem.EmitterSetRectangle(fireplace, PS::Vector2(256, 256), PS::Vector2(256, 8));
	partSystem.EmitterSetFrequency(fireplace, 0.00001f, 5);

	// Emitter #2
	/*PS::Emitter constFire = partSystem.CreateEmitter(fire);
	partSystem.EmitterSetCircle(constFire, PS::Vector2(64, 640), 16);
	partSystem.EmitterSetFrequency(constFire, 0.00001f);*/
	
	// FIRE 2
	/*-------------------------------------*/
	PS::Particle fire2 = partSystem.CreateParticle();
	partSystem.ParticleSetSize(fire2, 32, 64, 5);
	partSystem.ParticleSetRotation(fire2, 0.0f, 360.0f, 60.0f);
	partSystem.ParticleSetVelocity(fire2, PS::Vector2(64, 16));
	partSystem.ParticleSetColor(fire2, PS::Color(0, 255, 0, 125), PS::Color(0,0,255,0));
	partSystem.ParticleSetLifetime(fire2, 1.0f, 5.0f);

	// Emitter #1
	PS::Emitter fireplace2 = partSystem.CreateEmitter(fire2);
	partSystem.EmitterSetPoint(fireplace2, PS::Vector2(512.f, 256.f));
	partSystem.EmitterSetFrequency(fireplace2, 0.75f);

	// FIRE 3
	/*-------------------------------------*/
	PS::Particle fire3 = partSystem.CreateParticle();
	partSystem.ParticleSetSize(fire3, 4, 16, 5);
	partSystem.ParticleSetSpeed(fire3, 32, 128, 128);
	partSystem.ParticleSetRotation(fire3, 0.0f, 360.0f, 60.0f);
	partSystem.ParticleSetDirection(fire3, 270 + 32, 270 - 30);
	partSystem.ParticleSetColor(fire3, PS::Color(0, 255, 255, 255), PS::Color(0, 0, 255, 0));
	partSystem.ParticleSetLifetime(fire3, 1.0f, 3.0f);

	// Emitter #1
	PS::Emitter fireplace3 = partSystem.CreateEmitter(fire3);
	partSystem.EmitterSetPoint(fireplace3, PS::Vector2(1000.f, 640.f));
	partSystem.EmitterSetFrequency(fireplace3, 0.05f);

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
					partSystem.SpawnParticle(fire2, PS::Vector2((float)mousePos.x, (float)mousePos.y), 5);

					partSystem.DestroyParticle(fire2);
				}
				if (event.mouseButton.button == sf::Mouse::Button::Right)
				{
					partSystem.ClearVisibleParticles();
				}
			}
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		partSystem.EmitterSetLocation(fireplace, PS::Vector2((float)mousePos.x, (float)mousePos.y));

		//window.setMouseCursorVisible(false);

		// Clear screen
		window.clear();

#define TEXT_PARTICLE 0

#if TEXT_PARTICLE
		sf::Text textParticle;
		textParticle.setFont(fnt);
		textParticle.setStyle(sf::Text::Bold);
#endif

		// render particlesystem
		for (PS::ParticleIterator it(partSystem); it; it++)
		{
			PS::Output particle = (*it);//partSystem.GetParticle(i);
			PS::Vector2 location = particle.location;
			PS::Color color = particle.color;
			float size = particle.size;
			PS::Vector2 scale = particle.scale;
			scale *= 2;
			scale.Y = scale.X;
			sf::RectangleShape shape;

#if TEXT_PARTICLE

			textParticle.setString("+");
			textParticle.setPosition(location.X, location.Y);
			textParticle.setRotation(particle.rotation);
			textParticle.setScale(scale.X, scale.Y);
			textParticle.setColor(sf::Color(color.R, color.G, color.B, color.A));

			window.draw(textParticle);
#else

			shape.setPosition(location.X, location.Y);
			shape.setRotation(particle.rotation);
			shape.setScale(sf::Vector2f(scale.X, scale.Y));
			shape.setSize(sf::Vector2f(size, size));
			shape.setOrigin(size / 2.0f, size / 2.0f);
			shape.setFillColor(sf::Color(color.R, color.G, color.B, color.A));

			window.draw(shape);
#endif
		}

		sf::Text partCountLabel;
		partCountLabel.setStyle(sf::Text::Bold);
		partCountLabel.setFont(fnt);
		partCountLabel.setColor(sf::Color::White);
		partCountLabel.setString("Particles: " + std::to_string(partSystem.GetSpawnedParticleCount()));
		partCountLabel.setPosition(sf::Vector2f(32, 32));

		window.draw(partCountLabel);

		for (unsigned i = 0; i < partSystem.GetDefinitionCount(); i++)
		{
			partCountLabel.setString(std::to_string(i) + ": " + std::to_string(partSystem.GetSpawnedParticleTypeCount(i)));
			partCountLabel.setPosition(sf::Vector2f(32.0f, 96.0f + 32.0f*(float)i));

			window.draw(partCountLabel);
		}

		// FPS COUNTER
		int fps = (int)(1.0f / deltaTime);
		partCountLabel.setStyle(sf::Text::Bold);
		partCountLabel.setString(std::to_string(fps));
		partCountLabel.setPosition(1280 - 96 + 2, 64 + 2);
		partCountLabel.setColor(sf::Color::Black);
		window.draw(partCountLabel);
		partCountLabel.setColor(fps > 55 ? sf::Color::Green : sf::Color::Red);
		partCountLabel.setPosition(1280 - 96, 64);
		window.draw(partCountLabel);

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
}