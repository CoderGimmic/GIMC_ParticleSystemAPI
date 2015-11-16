// Emitter.cpp

#include "Emitter.h"

#include "ParticleSystem.h"

namespace GIMC
{
	/////////////////////////////////////////////////////////////////////
	// Emitter
	/////////////////////////////////////////////////////////////////////

	void Emitter::Burst(unsigned spawnedParticlesOverride)
	{
		owner->EmitterBurst(*this, spawnedParticlesOverride);
	}

	void Emitter::SetFrequency(float frequency, unsigned spawnCount, bool spawnImmediately)
	{
		owner->EmitterSetFrequency(*this, frequency, spawnCount, spawnImmediately);
	}

	void Emitter::SetLocation(Vector2 location)
	{
		owner->EmitterSetLocation(*this, location);
	}

	void Emitter::SetPoint(Vector2 location)
	{
		owner->EmitterSetPoint(*this, location);
	}

	void Emitter::SetCircle(float radius, Vector2 location)
	{
		owner->EmitterSetCircle(*this, radius, location);
	}

	void Emitter::SetRectangle(Vector2 dimension, Vector2 location)
	{
		owner->EmitterSetRectangle(*this, dimension, location);
	}

	void Emitter::SetRim(float thickness)
	{
		owner->EmitterSetRim(*this, thickness);
	}

	void Emitter::SetActive(bool state)
	{
		owner->EmitterSetActive(*this, state);
	}

	// Private

	Emitter::Emitter()
	{
		Reset();
	}

	Emitter::Emitter(ParticleSystem* system)
	{
		Reset();

		owner = system;
	}

	void Emitter::Reset()
	{
		owner = nullptr;
		uniqueID = (unsigned)-1;
		valid = false;
	}
};