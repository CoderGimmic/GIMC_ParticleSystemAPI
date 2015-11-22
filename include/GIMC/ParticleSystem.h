// ParticleSystem.h

#pragma once

#include "internal/ParticleDefinition.h"

#include "Particle.h"
#include "Settings.h"

////////////////////////////////////////////////////////////
//
//  "Graphics API indepenent Particle System Implementation"
//  Per "Gimmic" Johansson (http://codergimmic.wordpress.com)
//
////////////////////////////////////////////////////////////

namespace GIMC
{
	/////////////////////////////////////////////////////////////////////
	// ParticleSystem
	/////////////////////////////////////////////////////////////////////

	class EmitterDebugIterator;

	class ParticleSystem
	{
		friend class ParticleIterator;
		friend class EmitterDebugIterator;
		friend class EmitterDebugOutput;
		friend class Particle;
		friend class Emitter;

	public:

		ParticleSystem(ParticleSystemSettings& settings = ParticleSystemSettings());
		~ParticleSystem();

		void Update(float deltaTime);

		Particle CreateParticle();
		Emitter CreateEmitter(Particle spawnedParticle);
		void DestroyParticle(Particle& particle);
		void DestroyEmitter(Emitter emitter);

		void SpawnParticle(Particle particle, Vector2 location, unsigned spawnCount = 1);

		void ClearVisibleParticles();
		void ClearVisibleParticlesOfType(Particle& particle);

		unsigned GetParticleTypeCount();
		unsigned GetSpawnedParticleCountOfType(unsigned particle);
		unsigned GetSpawnedParticleCount();

		unsigned GetEmitterTypeCount(unsigned particle);
		unsigned GetEmitterCount();

		void Reset(unsigned newParticleLimit);

	private:

		void ParticleSetLifetime(Particle& particle, float minLife, float maxLife);
		
		void ParticleSetSize(Particle& particle, float sizeMin, float sizeMax, float sizeChange = 0.0f);
		void ParticleSetRotation(Particle& particle, float rotMin, float rotMax, float rotChange = 0.0f, bool rotRelative = false);
		void ParticleSetScale(Particle& particle, float scaleX, float scaleY);
		
		void ParticleSetColor(Particle& particle, Color colorStart, Color colorEnd);
		
		void ParticleSetSpeed(Particle& particle, float speedMin, float speedMax, float speedChange = 0.0f, bool clampToZero = false);
		void ParticleSetDirection(Particle& particle, float dirMin, float dirMax, float dirChange = 0.0f);
		void ParticleSetVelocity(Particle& particle, Vector2 velocity);
		void ParticleSetGravity(Particle& particle, float direction, float strength);
		
		void ParticleSetAttractorPoint(Particle& particle, Vector2 position, float strength, bool killOnCenter = false);
		void ParticleSetAttractorRange(Particle& particle, float range, bool linearfalloff = false);
		
		void ParticleSetRotatorPoint(Particle& particle, Vector2 position, bool useDegrees = true);
		void ParticleSetRotatorRange(Particle& particle, float range, bool linearFallof = false);
		
		void ParticleSetSpawnedParticle(Particle& particle, Particle spawnedParticle, unsigned numberOfSpawnedParticles = 1);
		void ParticleSetCustomData(Particle& particle, void* data);

		void ParticleReset(Particle& particle);

		void EmitterBurst(Emitter emitter, unsigned spawnedParticlesOverride = (unsigned)-1);

		void EmitterSetFrequency(Emitter emitter, float frequency, unsigned spawnCount = 1, bool spawnImmediately = false);
		
		void EmitterSetLocation(Emitter emitter, Vector2 location);
		void EmitterSetPoint(Emitter emitter, Vector2 location);
		void EmitterSetCircle(Emitter emitter, float radius, Vector2 location);
		void EmitterSetRectangle(Emitter emitter, Vector2 dimension, Vector2 location);
		void EmitterSetRim(Emitter, float thickness);
		
		void EmitterSetActive(Emitter emitter, bool state);

		internal::ParticleDef* getDefinitionFromIndexParticles(unsigned& index);
		internal::ParticleDef* getDefenitionFromIndexEmitters(unsigned& index);
		ParticleOutput* GetParticle(unsigned ParticleIndex);

		void CleanUp();

	private:

		unsigned numDefinitions;
		unsigned definitionLimit;
		internal::ParticleDef* particleDefinitions;

		unsigned emitterLimit;
		unsigned numParticles;
		unsigned numEmitters;

		static const unsigned MAX_DEFINITIONS;
	};
};