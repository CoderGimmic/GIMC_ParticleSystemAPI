// Math.cpp

#include "Math.h"

#include <random>

namespace GIMC
{
	/////////////////////////////////////////////////////////////////////
	// Random
	/////////////////////////////////////////////////////////////////////

	namespace
	{
		static std::mt19937 eng;
		static unsigned int engSeed = std::mt19937::default_seed;
	};

	namespace Random
	{
		unsigned int getSeed()
		{
			return engSeed;
		}

		void setSeed(unsigned int seed)
		{
			eng.seed(seed);
			engSeed = seed;
		}

		void setRandomSeed()
		{
			std::random_device rd;
			setSeed(rd());
		}

		int between(int min, int max)
		{
			std::uniform_int_distribution<int> dist(min, max);
			return dist(eng);
		}

		float random(float max)
		{
			std::uniform_real_distribution<float> dist(0.0f, max);
			return dist(eng);
		}

		float betweenf(float min, float max)
		{
			std::uniform_real_distribution<float> dist(min, max);
			return dist(eng);
		}
	};
};