// main.cpp

#include "SFML\Graphics\RenderWindow.hpp"
#include "SFML\Window\Event.hpp"

#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\Text.hpp"
#include "SFML\Graphics\Font.hpp"

#include "ParticleSystem.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/CircleShape.hpp"

#include "DebugGraph.h"

#include <iostream>
#include <string>

//#include <vld.h>

// TODO
/*---------
- change array to queue
- Emitter validity (passing as reference)
- particle updates on large timeframes (spawning multiple particles) substepping?
*/

float deltaTime = 0.0f;
sf::Clock deltaClock;

sf::Vector2i targetPosition = sf::Vector2i(0, 0);

void updateDeltatime()
{
	deltaTime = deltaClock.restart().asSeconds();
}

sf::Vector2f GetMousePosition(sf::RenderWindow* window, sf::Vector2f mousePos)
{
	window->setView(window->getDefaultView());
	return window->mapPixelToCoords(sf::Vector2i((int)mousePos.x, (int)mousePos.y));
}

int main(int argc, const char* argv[])
{
	// Create the main window
	int width = 1280;
	int height = 720;
	sf::RenderWindow window(
		sf::VideoMode(width, height), "ParticleSystemAPI - Demo");
	//window.setMouseCursorVisible(false);
	window.setFramerateLimit(60);

	sf::Font fnt;
	fnt.loadFromFile("../data/pixel.ttf");

	sf::Font graphFont;
	graphFont.loadFromFile("../data/courbd.ttf");

	DebugGraph fpsGraph("FPS");
	fpsGraph.setEntryCap(240);
	fpsGraph.setWarningThreshold(55);
	fpsGraph.toggle();

	DebugGraph deltaGraph("DeltaTime");
	deltaGraph.setEntryCap(240);
	//deltaGraph.setWarningThreshold(0.018f);
	deltaGraph.toggle();
	deltaGraph.setPosition(0.5f, 1.0f);

#define DRAW_PARTICLE 1
#define TEXT_PARTICLE 0
#define UPDATE_PARTICLE 1
#define BURST_TEST 0
#define SPARK_PARTICLE 0
#define DRAW_ADD 0

	PS::ParticleSystem partSystem;
	
	// FIRE
	/*-------------------------------------*/
	PS::Particle fire = partSystem.CreateParticle();
	partSystem.ParticleSetSize(fire, 2, 16, -10);
	//partSystem.ParticleSetScale(fire, 1.0f, 0.1f);
	partSystem.ParticleSetRotation(fire, 0.0f, 360.0f, 0.0f, 0, true);
	partSystem.ParticleSetSpeed(fire, 96, 128, 512);
	partSystem.ParticleSetDirection(fire, 270 - 32, 270 + 32, 90);
	partSystem.ParticleSetColor(fire, PS::Color(255, 255,0, 255), PS::Color(255,0,0,0));
	partSystem.ParticleSetLifetime(fire, 2.0f, 2.0f);

	// Emitter #1
	PS::Emitter fireplace = partSystem.CreateEmitter(fire);
	partSystem.EmitterSetPoint(fireplace, PS::Vector2(256.f, 256.f));
	partSystem.EmitterSetRectangle(fireplace, PS::Vector2(256, 8), PS::Vector2(256, 256));
	partSystem.EmitterSetPoint(fireplace, PS::Vector2());
	partSystem.EmitterSetCircle(fireplace, 64.0f, PS::Vector2());
	partSystem.EmitterSetFrequency(fireplace, 0.0001f, 5);
#if BURST_TEST 
	partSystem.EmitterSetActive(fireplace, false);
#endif

	// Emitter #2
	PS::Emitter constFire = partSystem.CreateEmitter(fire);
	partSystem.EmitterSetCircle(constFire, 16, PS::Vector2(64, 640));
	partSystem.EmitterSetFrequency(constFire, 0.05001f);

	// Spark
#if SPARK_PARTICLE
	PS::Particle spark = partSystem.CreateParticle();
	partSystem.ParticleSetColor(spark, PS::Color(255, 255, 0, 128), PS::Color(255, 255, 255, 64));
	partSystem.ParticleSetLifetime(spark, 0.1f, 0.3f);
	partSystem.ParticleSetLifetime(spark, 2.f, 4.f);
	partSystem.ParticleSetSize(spark, 0.5, 0.5, 0);

	partSystem.ParticleSetSpawnedParticle(fire, spark);

	// StarEmitter
	
	PS::Emitter starEmitter = partSystem.CreateEmitter(spark);
	partSystem.EmitterSetRectangle(starEmitter, 
		PS::Vector2(width / 2.0f, height / 2.0f), PS::Vector2(width / 2.0f, height / 2.0f));
	partSystem.EmitterSetFrequency(starEmitter, 8, 100, true);
#endif

#if 1
	// FIRE 2
	/*-------------------------------------*/
	PS::Particle fire2 = partSystem.CreateParticle();
	partSystem.ParticleSetSize(fire2, 32, 64, 5);
	partSystem.ParticleSetRotation(fire2, 0.0f, 360.0f, 60.0f);
	partSystem.ParticleSetVelocity(fire2, PS::Vector2(64, 16));
	partSystem.ParticleSetColor(fire2, PS::Color(0, 255, 255, 64), PS::Color(0,0,255,0));
	partSystem.ParticleSetLifetime(fire2, 1.0f, 5.0f);

	// Emitter #1
	PS::Emitter fireplace2 = partSystem.CreateEmitter(fire2);
	partSystem.EmitterSetPoint(fireplace2, PS::Vector2(512.f, 256.f));
	partSystem.EmitterSetFrequency(fireplace2, 0.75f);

	// FIRE 3
	/*-------------------------------------*/
	PS::Particle fire3 = partSystem.CreateParticle();
	partSystem.ParticleSetSize(fire3, 4, 16, 5);
	partSystem.ParticleSetSpeed(fire3, 32, 128, 0);
	partSystem.ParticleSetRotation(fire3, 0.0f, 360.0f, 60.0f);
	partSystem.ParticleSetDirection(fire3, 270 + 32, 270 - 30);
	partSystem.ParticleSetColor(fire3, PS::Color(0, 255, 255, 255), PS::Color(0, 0, 255, 0));
	partSystem.ParticleSetLifetime(fire3, 2.0f, 3.0f);
	partSystem.ParticleSetSpawnedParticle(fire3, fire);

	// Emitter #1
	PS::Emitter fireplace3 = partSystem.CreateEmitter(fire3);
	partSystem.EmitterSetPoint(fireplace3, PS::Vector2(1000.f, 640.f));
	partSystem.EmitterSetFrequency(fireplace3, 0.05f);
#endif

	float timer = 0;

	bool emitterState = false;
	bool LMB = false;
	bool RMB = false;

	bool num[9] = { false };

	bool debugEmitter = false;
	bool debugParticleOutline = false;
	bool enableFpsGraph = false;
	bool enableDeltaGraph = false;

	sf::Vector2f realMousePos = sf::Vector2f(0, 0);

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

					case sf::Keyboard::Numpad1:
						num[1] = true; 
						debugEmitter = !debugEmitter;
						break;
					case sf::Keyboard::Numpad2:
						num[2] = true;
						debugParticleOutline = !debugParticleOutline;
						break;
					case sf::Keyboard::Numpad3:
						enableFpsGraph = !enableFpsGraph;
						num[3] = true;
						break;
					case sf::Keyboard::Numpad4:
						enableDeltaGraph = !enableDeltaGraph;
						num[4] = true;
						break;
					case sf::Keyboard::Numpad5:
						num[5] = true;
						break;
				}
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Numpad1:
					num[1] = false;
					break;
				case sf::Keyboard::Numpad2:
					num[2] = false;
					break;
				case sf::Keyboard::Numpad3:
					num[3] = false;
					break;
				case sf::Keyboard::Numpad4:
					num[4] = false;
					break;
				case sf::Keyboard::Numpad5:
					num[5] = false;
					break;
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					LMB = true;

#if BURST_TEST
					partSystem.EmitterBurst(fireplace, 50);
#endif
				}
				if (event.mouseButton.button == sf::Mouse::Button::Right)
				{
					RMB = true;
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

					RMB = false;
				}
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				realMousePos.x = (float)event.mouseMove.x;
				realMousePos.y = (float)event.mouseMove.y;
			}
		}

		sf::Vector2f mousePos = GetMousePosition(&window, realMousePos);
		partSystem.EmitterSetLocation(fireplace, PS::Vector2(mousePos.x, mousePos.y));

#if !BURST_TEST
		partSystem.EmitterSetActive(fireplace, LMB);
#endif

		/////////////////////////////////////////////////////////////////////
		// Render
		/////////////////////////////////////////////////////////////////////

		// Clear screen
		window.clear();

#if DRAW_PARTICLE

	#if TEXT_PARTICLE
		sf::Text textParticle;
		textParticle.setFont(fnt);
		textParticle.setStyle(sf::Text::Bold);
	#endif

		// render particlesystem
		unsigned particleCount = 0;
		for (PS::ParticleIterator It(partSystem); It; It++)
		{
			PS::Output particle = (*It);

			PS::Vector2 location = particle.location;
			PS::Color color = particle.color;
			float size = particle.size;
			PS::Vector2 scale = particle.scale;

	#if TEXT_PARTICLE

			scale *= 2;
			textParticle.setString("+");
			textParticle.setPosition(location.X, location.Y);
			textParticle.setRotation(particle.rotation);
			textParticle.setScale(scale.X, scale.X);
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
			
			if (debugParticleOutline)
			{
				shape.setOutlineColor(sf::Color::Red);
				shape.setOutlineThickness(1.0f);
			}
#if DRAW_ADD
			sf::RenderStates rs; rs.blendMode = sf::BlendAdd;
			window.draw(shape, rs);
#else
			window.draw(shape);
#endif

	#endif

			particleCount++;
		}
#endif

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

		if (actualParticleCount != particleCount)
		{
			std::cout << "was: " << particleCount << " expected: " 
				<< actualParticleCount << std::endl;
		}

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
			debugLabel.setPosition(sf::Vector2f(96.0f, 128.0f + 32.0f*(float)i));

			window.draw(debugLabel);
		}

		debugLabel.setColor(sf::Color::Black);
		debugLabel.setString(std::to_string(fps));
		debugLabel.setPosition(sf::Vector2f(width - 64.0f + 2.0f, 32.0f));
		window.draw(debugLabel);
		debugLabel.setColor(fps > 55 ? sf::Color::Green : sf::Color::Red);
		debugLabel.setPosition(sf::Vector2f(width - 64.0f, 32.0f));
		window.draw(debugLabel);

		/////////////////////////////////////////////////////////////////////
		// Debug Emitter
		/////////////////////////////////////////////////////////////////////

		if (debugEmitter)
		{
			unsigned emitterCount = 0;
			for (PS::EmitterDebugIterator It(partSystem); It; It++)
			{
				PS::EmitterDebugOutput debugEmitter = (*It);
				PS::Vector2 location = debugEmitter.GetLocation();

				switch (debugEmitter.GetShape())
				{
					case PS::EmitterShape::POINT:
					{
						sf::CircleShape circle;
						circle.setPosition(location.X, location.Y);
						circle.setRadius(2.0f);
						circle.setFillColor(sf::Color::Red);
						circle.setOutlineColor(sf::Color::Red);
						window.draw(circle);
						break;
					}
					case PS::EmitterShape::CIRCLE:
					{
						sf::CircleShape circle;
						circle.setPosition(location.X, location.Y);
						float circleRadius = debugEmitter.GetCircleRadius();
						circle.setRadius(circleRadius);
						circle.setOrigin(sf::Vector2f(circleRadius, circleRadius));
						circle.setFillColor(sf::Color(255, 0, 0, 64));
						circle.setOutlineColor(sf::Color::Red);
						circle.setOutlineThickness(1);
						window.draw(circle);
						break;
					}

					case PS::EmitterShape::RECTANGLE:
					{
						sf::RectangleShape rect;
						rect.setPosition(location.X, location.Y);
						PS::Vector2 dim = debugEmitter.GetRectangleDimension();
						rect.setSize(sf::Vector2f(dim.X, dim.Y));
						rect.setOrigin(sf::Vector2f(dim.X / 2.0f, dim.Y / 2.0f));
						rect.setFillColor(sf::Color(255, 0, 0, 64));
						rect.setOutlineColor(sf::Color::Red);
						rect.setOutlineThickness(1);
						window.draw(rect);
						break;
					}
				}

				emitterCount++;
			}

			/*debugLabel.setString(std::to_string(emitterCount));
			debugLabel.setPosition(sf::Vector2f(8.0f, 8.0f));
			window.draw(debugLabel);*/
		}

		/////////////////////////////////////////////////////////////////////
		// Graphs
		/////////////////////////////////////////////////////////////////////

		// Fps graph
		if (enableFpsGraph)
		{
			fpsGraph.update((float)fps);
			fpsGraph.draw(&window, &graphFont, width, height);
		}

		// Fps graph
		if (enableDeltaGraph)
		{
			deltaGraph.update(deltaTime);
			deltaGraph.draw(&window, &graphFont, width, height);
		}

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
}