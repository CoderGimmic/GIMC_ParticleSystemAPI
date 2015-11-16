// Particle.h

#pragma once

#include "Color.h"
#include "Vector2.h"

namespace GIMC
{
	/////////////////////////////////////////////////////////////////////
	// Particle
	/////////////////////////////////////////////////////////////////////

	class Particle
	{
	public:
		friend class ParticleSystem;

		void SetLifetime(float minLife, float maxLife);

		void SetSize(float sizeMin, float sizeMax, float sizeChange = 0.0f);
		void SetRotation(float rotMin, float rotMax, float rotChange = 0.0f, bool rotRelative = false);
		void SetScale(float scaleX, float scaleY);

		void SetColor(Color color);
		void SetColor(Color colorStart, Color colorEnd);

		void SetSpeed(float speedMin, float speedMax, float speedChange = 0.0f, bool clampToZero = false);
		void SetDirection(float dirMin, float dirMax, float dirChange = 0.0f);
		void SetVelocity(Vector2 velocity);
		void SetGravity(float direction, float strength);

		void SetAttractorPoint(Vector2 position, float strength, bool killOnCenter = false);
		void SetAttractorRange(float range, bool linearFalloff = false);

		void SetRotatorPoint(Vector2 position, bool useDegrees = true);
		void SetRotatorRange(float range, bool linearFalloff = false);

		void SetSpawnedParticle(Particle& spawnedParticle, unsigned numberOfSpawnedParticles = 1);
		void SetCustomData(void* data);

		void Reset();

	private:

		Particle::Particle();
		Particle::Particle(ParticleSystem* system);

	private:

		bool valid;
		unsigned uniqueID;

		ParticleSystem* owner;
	};
};