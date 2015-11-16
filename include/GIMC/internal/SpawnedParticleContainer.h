// SpawnedParticleContainer.h

#pragma once

#include "Vector2.h"
#include "Settings.h"

namespace GIMC
{
	namespace internal
	{
		struct SpawnedParticleContainer
		{
			Vector2 locations[MAX_SPAWNCOUNT];
			unsigned size;

			SpawnedParticleContainer::SpawnedParticleContainer()
			{
				size = 0;
			}

			bool Add(Vector2 location)
			{
				if (size >= MAX_SPAWNCOUNT)
					return false;

				locations[size] = location;
				size++;

				return true;
			}
		};
	};
};