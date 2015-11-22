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

		void ParticleSetLifetime(unsigned particle, float minLife, float maxLife);
		
		void ParticleSetSize(unsigned particleID, float sizeMin, float sizeMax, float sizeChange = 0.0f);
		void ParticleSetRotation(unsigned particleID, float rotMin, float rotMax, float rotChange = 0.0f, bool rotRelative = false);
		void ParticleSetScale(unsigned particleID, float scaleX, float scaleY);
		
		void ParticleSetColor(unsigned particleID, Color colorStart, Color colorEnd);
		
		void ParticleSetSpeed(unsigned particleID, float speedMin, float speedMax, float speedChange = 0.0f, bool clampToZero = false);
		void ParticleSetDirection(unsigned particleID, float dirMin, float dirMax, float dirChange = 0.0f);
		void ParticleSetVelocity(unsigned particleID, Vector2 velocity);
		void ParticleSetGravity(unsigned particleID, float direction, float strength);
		
		void ParticleSetAttractorPoint(unsigned particleID, Vector2 position, float strength, bool killOnCenter = false);
		void ParticleSetAttractorRange(unsigned particleID, float range, bool linearfalloff = false);
		
		void ParticleSetRotatorPoint(unsigned particleID, Vector2 position, bool useDegrees = true);
		void ParticleSetRotatorRange(unsigned particleID, float range, bool linearFallof = false);
		
		void ParticleSetSpawnedParticle(unsigned particleID, unsigned spawnedParticleID, unsigned numberOfSpawnedParticles = 1);
		void ParticleSetCustomData(unsigned particleID, void* data);

		void ParticleReset(unsigned particleID);

		void EmitterBurst(unsigned emitterID, unsigned particleID, unsigned spawnedParticlesOverride = (unsigned)-1);

		void EmitterSetFrequency(unsigned emitterID, unsigned particleID, float frequency, unsigned spawnCount = 1, bool spawnImmediately = false);
		
		void EmitterSetLocation(unsigned emitterID, unsigned particleID, Vector2 location);
		void EmitterSetPoint(unsigned emitterID, unsigned particleID, Vector2 location);
		void EmitterSetCircle(unsigned emitterID, unsigned particleID, float radius, Vector2 location);
		void EmitterSetRectangle(unsigned emitterID, unsigned particleID, Vector2 dimension, Vector2 location);
		void EmitterSetRim(unsigned emitterID, unsigned particleID, float thickness);
		
		void EmitterSetActive(unsigned emitterID, unsigned particleID, bool state);

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