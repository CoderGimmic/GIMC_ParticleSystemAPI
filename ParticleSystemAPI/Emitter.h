// Emitter.h

#pragma once

#include "Vector2.h"

namespace PS
{
	/////////////////////////////////////////////////////////////////////
	// Emitter
	/////////////////////////////////////////////////////////////////////

	enum class EmitterShape
	{
		POINT,
		CIRCLE,
		RECTANGLE,
	};

	class Emitter
	{
	public:
		friend class ParticleSystem;

		void Burst(unsigned spawnedParticlesOverride = (unsigned)-1);

		void SetFrequency(float frequency, unsigned spawnCount = 1, bool spawnImmediately = false);

		void SetLocation(Vector2 location);
		void SetPoint(Vector2 location);
		void SetCircle(float radius, Vector2 location);
		void SetRectangle(Vector2 dimension, Vector2 location);
		void SetRim(float thickness);

		void SetActive(bool state);

	private:

		Emitter::Emitter();
		Emitter::Emitter(ParticleSystem* system);

		void Reset();

	private:

		unsigned uniqueID;
		unsigned particleID;

		bool valid;

		ParticleSystem* owner;
	};
};