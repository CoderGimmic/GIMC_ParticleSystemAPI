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
			friend class ParticleSystem;

		public:

			bool Add(Vector2 location)
			{
				if (size >= MAX_SPAWNCOUNT)
					return false;

				locations[size] = location;
				size++;

				return true;
			}

		private:

			SpawnedParticleContainer::SpawnedParticleContainer()
			{
				size = 0;
			}

		public:

			Vector2 locations[MAX_SPAWNCOUNT];
			unsigned size;
		};
	};
};