// EmitterDefinition.cpp

#include "EmitterDefinition.h"

#include "Math.h"

namespace PS
{
	namespace internal
	{
		/////////////////////////////////////////////////////////////////////
		// EmitterDef
		/////////////////////////////////////////////////////////////////////

		void EmitterDef::Reset()
		{
			location = Vector2(64, 64);
			shape = EmitterShape::POINT;

			dimension = Vector2(0.0f, 0.0f);
			rim = 0.0f;
			minRectRim = Vector2(0.0f, 0.0f);

			frequency = 1.0f;
			particleCount = 1;

			timer = 0.0f;
		}

		unsigned EmitterDef::Update(float deltaTime)
		{
			timer += deltaTime;
			if (timer >= frequency)
			{
				timer = 0.0f;
				return 1;
			}

			return 0;
		}

		Vector2 EmitterDef::GetSpawnLocation()
		{
			switch (shape)
			{
			case EmitterShape::POINT:
			{
				return location;
			}
				break;
			case EmitterShape::CIRCLE:
			{
				Vector2 point;

				float minRadius = rim == 0.0f ? 0.0f : dimension.X - rim;

				float angle = Random::betweenf(0.f, Math::PI2);
				float radius = Random::betweenf(minRadius, dimension.X);

				point.X = location.X + radius * cos(angle);
				point.Y = location.Y + radius * sin(angle);

				return point;
			}
				break;
			case EmitterShape::RECTANGLE:
			{
				if (rim == 0.0f)
				{
					return location +
						Vector2((dimension.X*-1.0f)*0.5f + Random::random(dimension.X),
						(dimension.Y*-1.0f)*0.5f + Random::random(dimension.Y));
				}
				else
				{
					float xPos, yPos;
					unsigned side = Random::between(1, 4);
					switch (side)
					{
						// Top
					case 1:
						xPos = (dimension.X*-1.0f)*0.5f + Random::random(dimension.X);
						yPos = Random::betweenf(minRectRim.Y, dimension.Y * 0.5f) * -1.0f;
						break;
						// Right
					case 2:
						xPos = Random::betweenf(minRectRim.X, dimension.X * 0.5f);
						yPos = (dimension.Y*-1.0f)*0.5f + Random::random(dimension.Y);
						break;
						// Down
					case 3:
						xPos = (dimension.X*-1.0f)*0.5f + Random::random(dimension.X);
						yPos = Random::betweenf(minRectRim.Y, dimension.Y * 0.5f);
						break;
						// Left
					case 4:
						xPos = Random::betweenf(minRectRim.X, dimension.X * 0.5f) * -1.0f;
						yPos = (dimension.Y*-1.0f)*0.5f + Random::random(dimension.Y);
						break;
					}

					return location + Vector2(xPos, yPos);
				}
			}
				break;
			}

			return Vector2();
		}

		void EmitterDef::InitTimer()
		{
			timer = frequency;
		}

		void EmitterDef::CalcRectRim()
		{
			if (rim == 0.0f)
				return;

			minRectRim.X = (dimension.X * 0.5f) - rim;
			minRectRim.Y = (dimension.Y * 0.5f) - rim;
		}

		// Private

		EmitterDef::EmitterDef()
		{
			Reset();
		}
	};
};