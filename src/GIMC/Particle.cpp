// Particle.cpp

#include "Particle.h"

#include "ParticleSystem.h"
#include "Color.h"

namespace GIMC
{
	/////////////////////////////////////////////////////////////////////
	// Particle
	/////////////////////////////////////////////////////////////////////

	void Particle::SetLifetime(float minLife, float maxLife)
	{
		if (valid == false)
			return;

		owner->ParticleSetLifetime(uniqueID, minLife, maxLife);
	}

	void Particle::SetSize(float sizeMin, float sizeMax, float sizeChange)
	{
		if (valid == false)
			return;

		owner->ParticleSetSize(uniqueID, sizeMin, sizeMax, sizeChange);
	}

	void Particle::SetRotation(float rotMin, float rotMax, float rotChange, bool rotRelative)
	{
		if (valid == false)
			return;

		owner->ParticleSetRotation(uniqueID, rotMin, rotMax, rotChange, rotRelative);
	}

	void Particle::SetScale(float scaleX, float scaleY)
	{
		if (valid == false)
			return;

		owner->ParticleSetScale(uniqueID, scaleX, scaleY);
	}

	void Particle::SetColor(Color color)
	{
		if (valid == false)
			return;

		owner->ParticleSetColor(uniqueID, color, color);
	}

	void Particle::SetColor(Color colorStart, Color colorEnd)
	{
		if (valid == false)
			return;

		owner->ParticleSetColor(uniqueID, colorStart, colorEnd);
	}

	void Particle::SetSpeed(float speedMin, float speedMax, float speedChange, bool clampToZero)
	{
		if (valid == false)
			return;

		owner->ParticleSetSpeed(uniqueID, speedMin, speedMax, speedChange, clampToZero);
	}

	void Particle::SetDirection(float dirMin, float dirMax, float dirChange)
	{
		if (valid == false)
			return;

		owner->ParticleSetDirection(uniqueID, dirMin, dirMax, dirChange);
	}

	void Particle::SetVelocity(Vector2 velocity)
	{
		if (valid == false)
			return;

		owner->ParticleSetVelocity(uniqueID, velocity);
	}

	void Particle::SetGravity(float direction, float strength)
	{
		if (valid == false)
			return;

		owner->ParticleSetGravity(uniqueID, direction, strength);
	}

	void Particle::SetAttractorPoint(Vector2 position, float strength, bool killOnCenter /* = false */)
	{
		if (valid == false)
			return;

		owner->ParticleSetAttractorPoint(uniqueID, position, strength, killOnCenter);
	}

	void Particle::SetAttractorRange(float range, bool linearFalloff /* = false */)
	{
		if (valid == false)
			return;

		owner->ParticleSetAttractorRange(uniqueID, range, linearFalloff);
	}

	void Particle::SetRotatorPoint(Vector2 position, bool useDegrees /* = true */)
	{
		if (valid == false)
			return;

		owner->ParticleSetRotatorPoint(uniqueID, position, useDegrees);
	}

	void Particle::SetRotatorRange(float range, bool linearFalloff)
	{
		if (valid == false)
			return;

		owner->ParticleSetRotatorRange(uniqueID, range, linearFalloff);
	}

	void Particle::SetSpawnedParticle(Particle& spawnedParticle, unsigned numberOfSpawnedParticles)
	{
		if (valid == false)
			return;

		if (spawnedParticle.valid == false)
			return;

		owner->ParticleSetSpawnedParticle(uniqueID, spawnedParticle.uniqueID, numberOfSpawnedParticles);
	}

	void Particle::SetCustomData(void* data)
	{
		if (valid == false)
			return;

		owner->ParticleSetCustomData(uniqueID, data);
	}

	void Particle::Reset()
	{
		if (valid == false)
			return;

		owner->ParticleReset(uniqueID);
	}

	// Private

	Particle::Particle()
	{}

	Particle::Particle(ParticleSystem* system)
		: valid(false)
		, uniqueID((unsigned)-1)
		, owner(system)
	{}
};