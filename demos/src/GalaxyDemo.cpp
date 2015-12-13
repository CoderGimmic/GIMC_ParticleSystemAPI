// GalaxyDemo.cpp

// GIMC_ParticleSystem
#include "GIMC_ParticleSystem.h"

// SFML Framework
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include <iostream>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif

float deltaTime = 0.0f;
sf::Clock deltaClock;

void updateDeltatime()
{
	deltaTime = deltaClock.restart().asSeconds();
}

int main(int argc, const char* argv[])
{
	std::string particleSystemVersionLabel =
		std::to_string(GIMC::internal::ParticleSystemVersion.major) + "." +
		std::to_string(GIMC::internal::ParticleSystemVersion.minor) + "." +
		std::to_string(GIMC::internal::ParticleSystemVersion.patch);

	// Create the main window
	int width = 1280;
	int height = 720;
	bool mouseVisible = true;
	sf::RenderWindow window(
		sf::VideoMode(width, height), "GIMC ParticleSystem ver " + particleSystemVersionLabel + " | GalaxyDemo");
	window.setMouseCursorVisible(mouseVisible);
	window.setFramerateLimit(60);

	std::string debugPathPrefix = "..";

#ifdef _WIN32

	if (IsDebuggerPresent() != FALSE)
	{
		debugPathPrefix += "/../../demos/";
	}

#endif

	sf::Font fnt;
	fnt.loadFromFile(debugPathPrefix + "/data/pixel.ttf");

	/////////////////////////////////////////////////////////////////////
	// GIMC Particle system
	/////////////////////////////////////////////////////////////////////

	GIMC::ParticleSystemSettings partSettings;
	//partSettings.particleLimit = 1000;

	GIMC::ParticleSystem partSystem(partSettings);

	// Star
	GIMC::Particle star = partSystem.CreateParticle();
	star.SetColor(GIMC::Color(255, 255, 0, 128), GIMC::Color(255, 255, 255, 0));
	star.SetLifetime(0.1f, 0.3f);
	star.SetLifetime(5.f, 10.f);
	star.SetSize(2, 2);

	// StarEmitter
	GIMC::Emitter space = partSystem.CreateEmitter(star);
	space.SetRectangle(GIMC::Vector2((float)width, (float)height), GIMC::Vector2((float)width / 2.0f, (float)height / 2.0f));
	space.SetFrequency(0.5f, 20, true);
	space.SetActive(true);

	GIMC::Vector2 middle = GIMC::Vector2(width / 2.0f, height / 2.0f);

	// Planet
	GIMC::Particle planet = partSystem.CreateParticle();

	planet.SetSize(4, 16, 3);
	planet.SetRotation(0.0f, 360.0f, 90.0f, false);
	planet.SetSpeed(24.0f, 32.0f);

	planet.SetColor(GIMC::Color(128, 64, 255, 0), GIMC::Color(64, 64, 255, 255));
	planet.SetLifetime(20, 20);
	planet.SetAttractorPoint(middle, 48, true);
	planet.SetRotatorPoint(middle);

	// Galaxy
	float galaxyFreq = 0.025f;
	unsigned planetCount = 2;
	float galaxySize = 320.0f;

	unsigned branches = 5;
	float angle = 0.0f;
	float angleInc = 360.0f / branches;
	for (unsigned i = 0; i < branches; i++)
	{
		GIMC::Emitter galaxy = partSystem.CreateEmitter(planet);
		galaxy.SetFrequency(galaxyFreq, planetCount);
		galaxy.SetPoint(middle + GIMC::Vector2::CreateUnit(angle) * galaxySize);

		angle += angleInc;
	}

	sf::Vector2f realMousePos = sf::Vector2f(0, 0);

	/////////////////////////////////////////////////////////////////////
	// Game loop start
	/////////////////////////////////////////////////////////////////////

	while (window.isOpen())
	{
		updateDeltatime();
		int fps = (int)(1.0f / deltaTime);

		// Simulate GIMC Particles
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
		}

		/////////////////////////////////////////////////////////////////////
		// Render
		/////////////////////////////////////////////////////////////////////

		window.clear();

		// Render GIMC Particles
		unsigned particleCount = 0;
		for (GIMC::ParticleIterator It(partSystem); It; It++)
		{
			GIMC::Output particle = (*It);

			GIMC::Vector2 location = particle.location;
			GIMC::Color color = particle.color;
			float size = particle.size;
			GIMC::Vector2 scale = particle.scale;

			sf::RectangleShape shape;
			shape.setPosition(location.X, location.Y);
			shape.setRotation(particle.rotation);
			shape.setScale(sf::Vector2f(scale.X, scale.Y));
			shape.setSize(sf::Vector2f(size, size));
			shape.setOrigin(size / 2.0f, size / 2.0f);
			shape.setFillColor(sf::Color(color.R, color.G, color.B, color.A));

			sf::RenderStates rs; rs.blendMode = sf::BlendAdd;
			window.draw(shape, rs);
		}

		/////////////////////////////////////////////////////////////////////
		// Text
		/////////////////////////////////////////////////////////////////////

		sf::Text debugLabel;
		debugLabel.setStyle(sf::Text::Bold);
		debugLabel.setFont(fnt);
		debugLabel.setColor(sf::Color::White);
		unsigned actualParticleCount = partSystem.GetSpawnedParticleCount();
		debugLabel.setString("Particles: " + std::to_string(actualParticleCount));
		debugLabel.setPosition(sf::Vector2f(32, 32));

		window.draw(debugLabel);

		debugLabel.setString("Emitters: " + std::to_string(partSystem.GetEmitterCount()));
		debugLabel.setPosition(sf::Vector2f(32, 64));

		window.draw(debugLabel);

		for (unsigned i = 0; i < partSystem.GetParticleTypeCount(); i++)
		{
			debugLabel.setString(std::to_string(i) + ": " + std::to_string(partSystem.GetSpawnedParticleCountOfType(i)));
			debugLabel.setPosition(sf::Vector2f(32.0f, 128.0f + 32.0f*(float)i));

			window.draw(debugLabel);

			debugLabel.setString(std::to_string(i) + ": " + std::to_string(partSystem.GetEmitterTypeCount(i)));
			debugLabel.setPosition(sf::Vector2f(128.0f, 128.0f + 32.0f*(float)i));

			window.draw(debugLabel);
		}

		debugLabel.setColor(sf::Color::Black);
		debugLabel.setString(std::to_string(fps));
		debugLabel.setPosition(sf::Vector2f(width - 64.0f + 2.0f, 32.0f));
		window.draw(debugLabel);
		debugLabel.setColor(fps > 55 ? sf::Color::Green : sf::Color::Red);
		debugLabel.setPosition(sf::Vector2f(width - 64.0f, 32.0f));
		window.draw(debugLabel);

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
}