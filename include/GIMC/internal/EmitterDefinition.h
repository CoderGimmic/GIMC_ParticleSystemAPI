// EmitterDefinition.h

#pragma once

#include "..\Vector2.h"
#include "..\Emitter.h"

namespace GIMC
{
	namespace internal
	{
		class EmitterDef
		{
			friend class ParticleSystem;
		public:

			void Reset();
			unsigned Update(float deltaTime);
			Vector2 GetSpawnLocation();
			void InitTimer();
			void CalcRectRim();

		public:

			EmitterDef();

			Vector2 location;
			EmitterShape shape;

			Vector2 dimension;
			float rim;
			Vector2 minRectRim;

			float frequency;
			unsigned particleCount;

		private:

			float timer;
		};
	};
};