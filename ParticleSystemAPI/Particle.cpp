// Particle.cpp

#include "Particle.h"

#include "ParticleSystem.h"
#include "Color.h"

namespace PS
{
	/////////////////////////////////////////////////////////////////////
	// Particle
	/////////////////////////////////////////////////////////////////////

	void Particle::SetLifetime(float minLife, float maxLife)
	{
		owner->ParticleSetLifetime(*this, minLife, maxLife);
	}

	void Particle::SetSize(float sizeMin, float sizeMax, float sizeChange)
	{
		owner->ParticleSetSize(*this, sizeMin, sizeMax, sizeChange);
	}

	void Particle::SetRotation(float rotMin, float rotMax, float rotChange, bool rotRelative)
	{
		owner->ParticleSetRotation(*this, rotMin, rotMax, rotChange, rotRelative);
	}

	void Particle::SetScale(float scaleX, float scaleY)
	{
		owner->ParticleSetScale(*this, scaleX, scaleY);
	}

	void Particle::SetColor(Color color)
	{
		owner->ParticleSetColor(*this, color, color);
	}

	void Particle::SetColor(Color colorStart, Color colorEnd)
	{
		owner->ParticleSetColor(*this, colorStart, colorEnd);
	}

	void Particle::SetSpeed(float speedMin, float speedMax, float speedChange, bool clampToZero)
	{
		owner->ParticleSetSpeed(*this, speedMin, speedMax, speedChange, clampToZero);
	}

	void Particle::SetDirection(float dirMin, float dirMax, float dirChange)
	{
		owner->ParticleSetDirection(*this, dirMin, dirMax, dirChange);
	}

	void Particle::SetVelocity(Vector2 velocity)
	{
		owner->ParticleSetVelocity(*this, velocity);
	}

	void Particle::SetGravity(float direction, float strength)
	{
		owner->ParticleSetGravity(*this, direction, strength);
	}

	void Particle::SetAttractorPoint(Vector2 position, float strength, bool killOnCenter /* = false */)
	{
		owner->ParticleSetAttractorPoint(*this, position, strength, killOnCenter);
	}

	void Particle::SetAttractorRange(float range, bool linearFalloff /* = false */)
	{
		owner->ParticleSetAttractorRange(*this, range, linearFalloff);
	}

	void Particle::SetRotatorPoint(Vector2 position, bool useDegrees /* = true */)
	{
		owner->ParticleSetRotatorPoint(*this, position, useDegrees);
	}

	void Particle::SetRotatorRange(float range, bool linearFalloff)
	{
		owner->ParticleSetRotatorRange(*this, range, linearFalloff);
	}

	void Particle::SetSpawnedParticle(Particle& spawnedParticle, unsigned numberOfSpawnedParticles)
	{
		owner->ParticleSetSpawnedParticle(*this, spawnedParticle, numberOfSpawnedParticles);
	}

	void Particle::SetCustomData(void* data)
	{
		owner->ParticleSetCustomData(*this, data);
	}

	void Particle::Reset()
	{
		owner->ParticleReset(*this);
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