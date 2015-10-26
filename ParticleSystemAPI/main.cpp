// main.cpp

#include "SFML\Graphics\RenderWindow.hpp"
#include "SFML\Window\Event.hpp"

#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\Text.hpp"
#include "SFML\Graphics\Font.hpp"

#include "ParticleSystem.h"
#include "SFML/Graphics/RectangleShape.hpp"

#include "DebugGraph.h"

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
	int width = 1280;
	int height = 720;
	sf::RenderWindow window(sf::VideoMode(width, height), "ParticleSystemAPI - Demo");
	//window.setMouseCursorVisible(false);
	//window.setFramerateLimit(60);

	sf::Font fnt;
	fnt.loadFromFile("../data/pixel.ttf");

	sf::Font graphFont;
	graphFont.loadFromFile("../data/courbd.ttf");

	DebugGraph fpsGraph("FPS");
	fpsGraph.setEntryCap(240);
	fpsGraph.setWarningThreshold(55);
	fpsGraph.toggle();

#define ENABLE_FPSGRAPH 1
#define DRAW_PARTICLE 1
#define TEXT_PARTICLE 0
#define UPDATE_PARTICLE 1

	PS::ParticleSystem partSystem;
	
	// FIRE
	/*-------------------------------------*/
	PS::Particle fire = partSystem.CreateParticle();
	partSystem.ParticleSetSize(fire, 1, 1, 5);
	partSystem.ParticleSetScale(fire, 1.0f, 0.1f);
	partSystem.ParticleSetRotation(fire, 0.0f, 360.0f, 0.0f, 0, false);
	partSystem.ParticleSetSpeed(fire, 96, 128, 96);
	partSystem.ParticleSetDirection(fire, 270 - 32, 270 + 32, 45);
	partSystem.ParticleSetColor(fire, PS::Color(255, 0,255, 255), PS::Color(255,255,0,0));
	partSystem.ParticleSetLifetime(fire, 2.0f, 2.0f);

	// Emitter #1
	PS::Emitter fireplace = partSystem.CreateEmitter(fire);
	partSystem.EmitterSetPoint(fireplace, PS::Vector2(256.f, 256.f));
	partSystem.EmitterSetRectangle(fireplace, PS::Vector2(256, 256), PS::Vector2(256, 8));
	partSystem.EmitterSetPoint(fireplace, PS::Vector2());
	partSystem.EmitterSetFrequency(fireplace, 0.0001f, 5);
	partSystem.EmitterSetActive(fireplace, false);

	// Emitter #2
	PS::Emitter constFire = partSystem.CreateEmitter(fire);
	partSystem.EmitterSetCircle(constFire, PS::Vector2(64, 640), 16);
	partSystem.EmitterSetFrequency(constFire, 0.05001f);
	
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
	partSystem.ParticleSetLifetime(fire3, 2.0f, 3.0f);

	// Emitter #1
	PS::Emitter fireplace3 = partSystem.CreateEmitter(fire3);
	partSystem.EmitterSetPoint(fireplace3, PS::Vector2(1000.f, 640.f));
	partSystem.EmitterSetFrequency(fireplace3, 0.05f);

	float timer = 0;

	bool emitterState = false;
	bool LMB = false;
	bool RMB = false;

	while (window.isOpen())
	{
		updateDeltatime();
		timer += deltaTime;
		int fps = (int)(1.0f / deltaTime);

#if UPDATE_PARTICLE
		partSystem.Update(deltaTime);
#endif

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
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					LMB = true;
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					LMB = false;
				}
				if (event.mouseButton.button == sf::Mouse::Button::Right)
				{
					partSystem.ClearVisibleParticles();
				}
			}
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		partSystem.EmitterSetLocation(fireplace, PS::Vector2((float)mousePos.x, (float)mousePos.y));

		partSystem.EmitterSetActive(fireplace, LMB);

		// Clear screen
		window.clear();

#if DRAW_PARTICLE

	#if TEXT_PARTICLE
		sf::Text textParticle;
		textParticle.setFont(fnt);
		textParticle.setStyle(sf::Text::Bold);
	#endif

		// render particlesystem
		for (PS::ParticleIterator it(partSystem); it; it++)
		{
			PS::Output particle = (*it);

			PS::Vector2 location = particle.location;
			PS::Color color = particle.color;
			float size = particle.size;
			PS::Vector2 scale = particle.scale;
			scale *= 2;
			scale.Y = scale.X;

	#if TEXT_PARTICLE

			textParticle.setString("+");
			textParticle.setPosition(location.X, location.Y);
			textParticle.setRotation(particle.rotation);
			textParticle.setScale(scale.X, scale.Y);
			textParticle.setColor(sf::Color(color.R, color.G, color.B, color.A));

			window.draw(textParticle);
	#else
			sf::RectangleShape shape;

			shape.setPosition(location.X, location.Y);
			shape.setRotation(particle.rotation);
			shape.setScale(sf::Vector2f(scale.X, scale.Y));
			shape.setSize(sf::Vector2f(size, size));
			shape.setOrigin(size / 2.0f, size / 2.0f);
			shape.setFillColor(sf::Color(color.R, color.G, color.B, color.A));

			window.draw(shape);
	#endif
		}

#endif

		sf::Text debugLabel;
		debugLabel.setStyle(sf::Text::Bold);
		debugLabel.setFont(fnt);
		debugLabel.setColor(sf::Color::White);
		debugLabel.setString("Particles: " + std::to_string(partSystem.GetSpawnedParticleCount()));
		debugLabel.setPosition(sf::Vector2f(32, 32));

		window.draw(debugLabel);

		for (unsigned i = 0; i < partSystem.GetParticleTypeCount(); i++)
		{
			debugLabel.setString(std::to_string(i) + ": " + std::to_string(partSystem.GetSpawnedParticleCountOfType(i)));
			debugLabel.setPosition(sf::Vector2f(32.0f, 96.0f + 32.0f*(float)i));

			window.draw(debugLabel);
		}

		debugLabel.setColor(sf::Color::Black);
		debugLabel.setString(std::to_string(fps));
		debugLabel.setPosition(sf::Vector2f(width - 64 + 2, 32));
		window.draw(debugLabel);
		debugLabel.setColor(fps > 55 ? sf::Color::Green : sf::Color::Red);
		debugLabel.setPosition(sf::Vector2f(width - 64, 32));
		window.draw(debugLabel);

		// Fps graph
#if ENABLE_FPSGRAPH
		fpsGraph.update(fps);
		fpsGraph.draw(&window, &graphFont, width, height);
#endif

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
}