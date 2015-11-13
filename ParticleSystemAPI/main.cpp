// main.cpp

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Graphics/Sprite.hpp"

#include "ParticleSystem.h"

#include "DebugGraph.h"

#include <iostream>
#include <string>

//#include <vld.h>

/*
	Base life on parameteter?

*/
float deltaTime = 0.0f;
sf::Clock deltaClock;

sf::Vector2i targetPosition = sf::Vector2i(0, 0);

void updateDeltatime()
{
	deltaTime = deltaClock.restart().asSeconds();
}

sf::Vector2f GetMousePositionRelativeToWindow(sf::RenderWindow* window, sf::Vector2f mousePos)
{
	window->setView(window->getDefaultView());
	return window->mapPixelToCoords(sf::Vector2i((int)mousePos.x, (int)mousePos.y));
}

struct ParticleCustomData
{
	enum class EParticleType
	{
		Sprite,
		Text,
		Square,
	};

	ParticleCustomData::ParticleCustomData(EParticleType _type, void* _data)
		: data(_data)
		, type(_type)
	{}

	void* data;
	EParticleType type;
};

int main(int argc, const char* argv[])
{
	// Create the main window
	int width = 1280;
	int height = 720;
	bool mouseVisible = true;
	sf::RenderWindow window(
		sf::VideoMode(width, height), "ParticleSystemAPI - Demo");
	window.setMouseCursorVisible(mouseVisible);
	window.setFramerateLimit(60);

	sf::Font fnt;
	fnt.loadFromFile("../data/pixel.ttf");

	sf::Font graphFont;
	graphFont.loadFromFile("../data/courbd.ttf");

	sf::Texture flareTexture;
	flareTexture.loadFromFile("../data/flare.png");
	sf::Texture cursorTexture;
	cursorTexture.loadFromFile("../data/spr_cursor.png");
	sf::Texture symbolTexture;
	symbolTexture.loadFromFile("../data/spr_console.png");

	sf::Sprite flare;
	flare.setTexture(flareTexture);
	flare.setOrigin(flareTexture.getSize().x / 2.0f, flareTexture.getSize().y / 2.0f);

	sf::Sprite cursor;
	cursor.setTexture(cursorTexture);
	cursor.setOrigin(cursorTexture.getSize().x / 2.0f, cursorTexture.getSize().y / 2.0f);

	sf::Sprite symbol;
	symbol.setTexture(symbolTexture);
	symbol.setOrigin(symbolTexture.getSize().x / 2.0f, symbolTexture.getSize().y / 2.0f);

	DebugGraph fpsGraph("FPS");
	fpsGraph.setEntryCap(240);
	fpsGraph.setWarningThreshold(55);
	fpsGraph.toggle();
	fpsGraph.setPosition(0.25f, 1.0f);

	DebugGraph deltaGraph("DeltaTime");
	deltaGraph.setEntryCap(240);
	deltaGraph.setWarningThreshold(1 / 55.0f, true);
	deltaGraph.toggle();
	deltaGraph.setPosition(0.75f, 1.0f);

#define DRAW_PARTICLE 1
#define UPDATE_PARTICLE 1

#define CUSTOM_PARTICLE 0 // NOTE(Per): Test. Don't touch!
#define GRAPHIC_PARTICLE 0 // Text/Sprite
#define TEXT_PARTICLE 0

#define BURST_TEST 0
#define SPARK_PARTICLE 0
#define DRAW_ADD 1

	sf::String textString("@");
	PS::ParticleSystem partSystem;

	PS::Particle test = partSystem.CreateParticle();
	
	// FIRE
	/*-------------------------------------*/
	PS::Particle fire = partSystem.CreateParticle();
	fire.SetSize(2, 16, -10);
	//fire.SetScale(1.0f, 0.1f);
	fire.SetRotation(0.0f, 360.0f, 0.0f, false);
	fire.SetSpeed(96, 512, -64);
	fire.SetDirection(270 - 32, 270 + 32, 90);
	//fire.SetGravity(90, 128);
	fire.SetColor(PS::Color(255, 255, 0, 255), PS::Color(255, 0, 0, 0));
	fire.SetColor(PS::Color::Yellow, PS::Color(0, 0, 255, 0));
	//fire.SetColor(PS::Color::Green, PS::Color(0, 255, 0, 0));
	fire.SetLifetime(1.0f, 2.0f);
	fire.SetCustomData(&flare);
#if CUSTOM_PARTICLE
	fire.SetCustomData(&ParticleCustomData(ParticleCustomData::EParticleType::Sprite, &flare));
#endif

	// Emitter #1
	PS::Emitter fireplace = partSystem.CreateEmitter(fire);
	fireplace.SetPoint(PS::Vector2(256.f, 256.f));
	fireplace.SetPoint(PS::Vector2());
	fireplace.SetCircle(64.0f, PS::Vector2());
	fireplace.SetRectangle(PS::Vector2(256, 96), PS::Vector2(256, 256));
	fireplace.SetRim(16);
	fireplace.SetFrequency(0.0001f, 5);
#if BURST_TEST 
	fireplace.SetActive(false);
#endif

	// Emitter #2
	PS::Emitter constFire = partSystem.CreateEmitter(fire);
	constFire.SetCircle(16, PS::Vector2(64, 640));
	constFire.SetFrequency(0.05001f);

	// Spark
#if SPARK_PARTICLE
	PS::Particle spark = partSystem.CreateParticle();
	spark.SetColor(PS::Color(255, 255, 0, 128), PS::Color(255, 255, 255, 0));
	spark.SetLifetime(0.1f, 0.3f);
	spark.SetLifetime(2.f, 3.f);
	spark.SetSize(0.5, 0.5, 5);

	fire.SetSpawnedParticle(spark);

	// StarEmitter
#if 1
	PS::Emitter starEmitter = partSystem.CreateEmitter(spark);
	starEmitter.SetRectangle(PS::Vector2(width / 2.0f, height / 2.0f), PS::Vector2(width / 2.0f, height / 2.0f));
	starEmitter.SetFrequency(8, 100, true);
	starEmitter.SetActive(false);
#endif
#endif

#if 0
#if 1
	// FIRE 2
	/*-------------------------------------*/
	PS::Particle fire2 = partSystem.CreateParticle();
	fire2.SetSize(32, 64, 5);
	fire2.SetRotation(0.0f, 360.0f, 60.0f);
	fire2.SetVelocity(PS::Vector2(64, 16));
	fire2.SetColor(PS::Color(0, 255, 255, 128), PS::Color(0, 0, 255, 0));
	fire2.SetLifetime(1.0f, 5.0f);
	fire2.SetCustomData(&cursor);
#if CUSTOM_PARTICLE
	fire2.SetCustomData(&ParticleCustomData(ParticleCustomData::EParticleType::Text, nullptr));
#endif

	// Emitter #1
	PS::Emitter fireplace2 = partSystem.CreateEmitter(fire2);
	fireplace2.SetPoint(PS::Vector2(512.f, 256.f));
	fireplace2.SetFrequency(0.75f);

#endif

	// FIRE 3
	/*-------------------------------------*/
	PS::Particle fire3 = partSystem.CreateParticle();
	fire3.SetSize(4, 16, 5);
	fire3.SetSpeed(32, 128, 0);
	fire3.SetRotation(0.0f, 360.0f, 60.0f);
	fire3.SetDirection(270 + 32, 270 - 30);
	fire3.SetColor(PS::Color(0, 255, 255, 255), PS::Color(0, 0, 255, 0));
	fire3.SetLifetime(2.0f, 3.0f);
	fire3.SetCustomData(&symbol);
#if CUSTOM_PARTICLE
	fire3.SetCustomData(&ParticleCustomData(ParticleCustomData::EParticleType::Square, nullptr));
#endif

	// Emitter #1
	PS::Emitter fireplace3 = partSystem.CreateEmitter(fire3);
	fireplace3.SetPoint(PS::Vector2(1000.f, 640.f));
	fireplace3.SetRectangle(PS::Vector2(128.0f, 128.0f), PS::Vector2(1000.f, 512.f));
	fireplace3.SetFrequency(0.0005f);
	fireplace3.SetRim(8.0f);
#endif

	PS::Vector2 middle = PS::Vector2(width / 2, height / 2);

	PS::Particle planet = partSystem.CreateParticle();
	planet.SetSize(4, 16/*, 16*/);
	//planet.SetScale(0.25, 0.25);
	planet.SetRotation(0.0f, 360.0f, 0.0f, false);
	planet.SetSpeed(48.0f, 64.0f);
	//planet.SetDirection(0, 0, 0.000001f);
	//planet.SetGravity(360, 64);
	planet.SetColor(PS::Color(128, 64, 255, 0), PS::Color(64, 64, 255, 255));
	//planet.SetColor(PS::Color(255,255,255,0), PS::Color::White);
	planet.SetLifetime(4, 4);
	planet.SetAttractorPoint(middle, 96 );
	planet.SetRotatorPoint(middle - PS::Vector2(0, 48));
	planet.SetCustomData(&flare);
	//planet.SetSpawnedParticle(fire);

#if 1
	float galaxyFreq = 0.025f;
	float galaxySize = 320.0f;

	unsigned branches = 5;
	float angle = 0.0f;
	float angleInc = 360.0f / branches;
	for (unsigned i = 0; i < branches; i++)
	{
		PS::Emitter galaxy = partSystem.CreateEmitter(planet);
		galaxy.SetFrequency(galaxyFreq, 3);
		galaxy.SetPoint(middle + PS::Vector2::CreateUnit(angle) * galaxySize);

		angle += angleInc;
	}

	PS::Emitter middlePoint = partSystem.CreateEmitter(planet);
	middlePoint.SetPoint(middle);
	middlePoint.SetActive(false);

#else
	PS::Emitter galaxy = partSystem.CreateEmitter(planet);
	galaxy.SetCircle(256, PS::Vector2(width / 2, height / 2));
	//galaxy.SetPoint(PS::Vector2(width / 2, height / 2));
	//galaxy.SetRim(16);
	galaxy.SetFrequency(0.015f, 1);
	//galaxy.SetFrequency(1);
#endif

	float timer = 0;

	bool emitterState = false;
	bool LMB = false;
	bool RMB = false;
	bool MMB = false;
	bool X1MB = false;
	bool X2MB = false;

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
					fireplace.Burst(50);
#endif
				}
				if (event.mouseButton.button == sf::Mouse::Button::Right)
				{
					RMB = true;
				}
				if (event.mouseButton.button == sf::Mouse::Button::Middle)
				{
					MMB = true;
					mouseVisible = !mouseVisible;
					window.setMouseCursorVisible(mouseVisible);
				}
				if (event.mouseButton.button == sf::Mouse::Button::XButton1)
				{
					X1MB = true;
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
					//partSystem.ClearVisibleParticlesOfType(fire);

					RMB = false;
				}
				if (event.mouseButton.button == sf::Mouse::Button::Middle)
				{
					MMB = true;
				}
				if (event.mouseButton.button == sf::Mouse::Button::XButton1)
				{
					X1MB = false;
				}
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				realMousePos.x = (float)event.mouseMove.x;
				realMousePos.y = (float)event.mouseMove.y;
			}
		}

		sf::Vector2f mousePos = GetMousePositionRelativeToWindow(&window, realMousePos);
		fireplace.SetLocation(PS::Vector2(mousePos.x, mousePos.y));

		if (X1MB)
		{
			sf::Vector2f mp = GetMousePositionRelativeToWindow(&window, realMousePos);
			planet.SetRotatorPoint(PS::Vector2(mp.x, mp.y));
		}

#if !BURST_TEST
		fireplace.SetActive(LMB);
#endif

		/////////////////////////////////////////////////////////////////////
		// Render
		/////////////////////////////////////////////////////////////////////

		window.clear();

	#if TEXT_PARTICLE
		sf::Text textParticle;
		textParticle.setFont(graphFont);
		textParticle.setStyle(sf::Text::Bold);
	#endif

		// render particlesystem
		unsigned particleCount = 0;
		for (PS::ParticleIterator It(partSystem); It; It++)
		{
#if DRAW_PARTICLE
			PS::Output particle = (*It);

			PS::Vector2 location = particle.location;
			PS::Color color = particle.color;
			float size = particle.size;
			PS::Vector2 scale = particle.scale;
#if CUSTOM_PARTICLE

			if (particle.customData == nullptr)
				continue;

			ParticleCustomData::EParticleType type =
				static_cast<ParticleCustomData>(*(ParticleCustomData*)particle.customData).type;

			switch(type)
			{
			case ParticleCustomData::EParticleType::Square:
			{
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

				break;
			}
			case ParticleCustomData::EParticleType::Text:
			{
				textParticle.setCharacterSize(size*3.0f);
				textParticle.setString(textString);
				textParticle.setPosition(location.X, location.Y);
				textParticle.setRotation(particle.rotation);
				textParticle.setScale(scale.X, scale.X);
				textParticle.setColor(sf::Color(color.R, color.G, color.B, color.A));

				window.draw(textParticle);
				break;
			}
			case ParticleCustomData::EParticleType::Sprite:
			{
				sf::Sprite* spriteParticle = (sf::Sprite*)
					static_cast<ParticleCustomData>(*(ParticleCustomData*)particle.customData).data;

				if (spriteParticle)
				{
					spriteParticle->setPosition(sf::Vector2f(location.X, location.Y));
					spriteParticle->setScale(scale.X, scale.Y);
					spriteParticle->setRotation(particle.rotation);
					spriteParticle->setColor(sf::Color(color.R, color.G, color.B, color.A));

					window.draw(*spriteParticle);
				}
				break;
			}
				default:
					break;
			}
#else

	#if GRAPHIC_PARTICLE
#if TEXT_PARTICLE
			textParticle.setCharacterSize(size*3.0f);
			textParticle.setString(textString);
			textParticle.setPosition(location.X, location.Y);
			textParticle.setRotation(particle.rotation);
			textParticle.setScale(scale.X, scale.X);
			textParticle.setColor(sf::Color(color.R, color.G, color.B, color.A));

			window.draw(textParticle);
#else
			sf::Sprite* spriteParticle = (sf::Sprite*)particle.customData;

			if (spriteParticle)
			{
				spriteParticle->setPosition(sf::Vector2f(location.X, location.Y));
				spriteParticle->setScale(scale.X, scale.Y);
				spriteParticle->setRotation(particle.rotation);
				spriteParticle->setColor(sf::Color(color.R, color.G, color.B, color.A));

#if DRAW_ADD
				sf::RenderStates rs; rs.blendMode = sf::BlendAdd;
				window.draw(*spriteParticle, rs);
#else
				window.draw(*spriteParticle);
#endif
			}
#endif
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
#endif
#endif
			particleCount++;
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

#if DRAW_PARTICLE
		if (actualParticleCount != particleCount)
		{
			std::cout << "was: " << particleCount << " expected: " 
				<< actualParticleCount << std::endl;
		}
#endif

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
				bool activeState = debugEmitter.GetActive();

				sf::Color outlineColor = activeState ? sf::Color(255, 255, 255) : sf::Color(255, 128, 128);
				sf::Color fillColor = activeState ? sf::Color(255, 255, 255, 64) : sf::Color(255, 128, 128, 64);

				switch (debugEmitter.GetShape())
				{
					case PS::EmitterShape::POINT:
					{
						float crossSize = 16.f;
						sf::Vertex horLine[2] =
						{
							sf::Vertex(sf::Vector2f(location.X + crossSize, location.Y), outlineColor),
							sf::Vertex(sf::Vector2f(location.X - crossSize, location.Y), outlineColor)
						};
						window.draw(horLine, 2, sf::Lines);

						sf::Vertex vertLine[2] =
						{
							sf::Vertex(sf::Vector2f(location.X, location.Y + crossSize), outlineColor),
							sf::Vertex(sf::Vector2f(location.X, location.Y - crossSize), outlineColor)
						};
						window.draw(vertLine, 2, sf::Lines);

						break;
					}
					case PS::EmitterShape::CIRCLE:
					{
						float rim = debugEmitter.GetRim();
						if (rim == 0.0f)
						{
							sf::CircleShape circle;
							circle.setPosition(location.X, location.Y);
							float circleRadius = debugEmitter.GetCircleRadius();
							circle.setRadius(circleRadius);
							circle.setOrigin(sf::Vector2f(circleRadius, circleRadius));
							circle.setFillColor(fillColor);
							circle.setOutlineColor(outlineColor);
							circle.setOutlineThickness(1.0f);
							window.draw(circle);
						}
						else
						{
							float circleRadius = debugEmitter.GetCircleRadius();
							float innerRadius = circleRadius - rim;

							sf::CircleShape circle;
							circle.setPosition(location.X, location.Y);
							circle.setFillColor(sf::Color::Transparent);
							circle.setOutlineColor(outlineColor);

							circle.setRadius(innerRadius);
							circle.setOrigin(sf::Vector2f(innerRadius, innerRadius));
							circle.setOutlineThickness(1.0f);
							window.draw(circle);

							circle.setRadius(circleRadius);
							circle.setOrigin(sf::Vector2f(circleRadius, circleRadius));
							window.draw(circle);

							float thickRadius = (innerRadius + circleRadius) * 0.5f;
							thickRadius -= rim * 0.5f;

							circle.setOutlineColor(fillColor);
							circle.setOutlineThickness(rim);
							circle.setRadius(thickRadius);
							circle.setOrigin(sf::Vector2f(thickRadius, thickRadius));
							window.draw(circle);
						}

						break;
					}
					case PS::EmitterShape::RECTANGLE:
					{
						float rim = debugEmitter.GetRim();
						if (rim == 0.0f)
						{
							sf::RectangleShape rect;
							rect.setPosition(location.X, location.Y);
							PS::Vector2 dim = debugEmitter.GetRectangleDimension();
							rect.setSize(sf::Vector2f(dim.X, dim.Y));
							rect.setOrigin(sf::Vector2f(dim.X * 0.5f, dim.Y * 0.5f));
							rect.setFillColor(fillColor);
							rect.setOutlineColor(outlineColor);
							rect.setOutlineThickness(1.0f);
							window.draw(rect);
						}
						else
						{
							PS::Vector2 dim = debugEmitter.GetRectangleDimension();
							PS::Vector2 innerDim(dim.X - rim * 2.0, dim.Y - rim * 2.0);

							sf::RectangleShape rect;
							rect.setPosition(location.X, location.Y);
							rect.setFillColor(sf::Color::Transparent);
							rect.setOutlineColor(outlineColor);

							rect.setSize(sf::Vector2f(innerDim.X, innerDim.Y));
							rect.setOrigin(sf::Vector2f(innerDim.X * 0.5f, innerDim.Y * 0.5f));
							rect.setOutlineThickness(1.0f);
							window.draw(rect);

							rect.setSize(sf::Vector2f(dim.X, dim.Y));
							rect.setOrigin(sf::Vector2f(dim.X * 0.5f, dim.Y * 0.5f));
							rect.setOutlineThickness(1.0f);
							window.draw(rect);

							PS::Vector2 thickDim = PS::Vector2(dim + innerDim) * 0.5f;
							thickDim.X -= rim;
							thickDim.Y -= rim;

							rect.setOutlineColor(fillColor);
							rect.setOutlineThickness(rim);
							rect.setSize(sf::Vector2f(thickDim.X, thickDim.Y));
							rect.setOrigin(sf::Vector2f(thickDim.X * 0.5f, thickDim.Y * 0.5f));
							window.draw(rect);
						}
						break;
					}
				}

				emitterCount++;
			}

#if 0
			debugLabel.setString(std::to_string(emitterCount));
			debugLabel.setPosition(sf::Vector2f(8.0f, 8.0f));
			window.draw(debugLabel);
#endif
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