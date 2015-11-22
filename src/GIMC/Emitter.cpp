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
		if (valid == false)
			return;

		owner->EmitterBurst(uniqueID, particleID, spawnedParticlesOverride);
	}

	void Emitter::SetFrequency(float frequency, unsigned spawnCount, bool spawnImmediately)
	{
		if (valid == false)
			return;

		owner->EmitterSetFrequency(uniqueID, particleID, frequency, spawnCount, spawnImmediately);
	}

	void Emitter::SetLocation(Vector2 location)
	{
		if (valid == false)
			return;

		owner->EmitterSetLocation(uniqueID, particleID, location);
	}

	void Emitter::SetPoint(Vector2 location)
	{
		if (valid == false)
			return;

		owner->EmitterSetPoint(uniqueID, particleID, location);
	}

	void Emitter::SetCircle(float radius, Vector2 location)
	{
		if (valid == false)
			return;

		owner->EmitterSetCircle(uniqueID, particleID, radius, location);
	}

	void Emitter::SetRectangle(Vector2 dimension, Vector2 location)
	{
		if (valid == false)
			return;

		owner->EmitterSetRectangle(uniqueID, particleID, dimension, location);
	}

	void Emitter::SetRim(float thickness)
	{
		if (valid == false)
			return;

		owner->EmitterSetRim(uniqueID, particleID, thickness);
	}

	void Emitter::SetActive(bool state)
	{
		if (valid == false)
			return;

		owner->EmitterSetActive(uniqueID, particleID, state);
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