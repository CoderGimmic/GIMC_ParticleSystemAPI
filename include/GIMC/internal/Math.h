// Math.h

#pragma once

#include <random>

namespace GIMC
{
	namespace internal
	{
		namespace Math
		{
			const static float PI = 3.14159265358979323846f;
			const static float PI2 = 6.28318530717958647693f;
			const static float degToRad = 0.01745329251994329576f;
			const static float radToDeg = 57.2957795131f;
		};

		/////////////////////////////////////////////////////////////////////
		// Random
		/////////////////////////////////////////////////////////////////////

		namespace Random
		{
			unsigned int getSeed();

			void setSeed(unsigned int seed);

			void setRandomSeed();

			int between(int min, int max);

			float random(float max);

			float betweenf(float min, float max);
		};
	}
};