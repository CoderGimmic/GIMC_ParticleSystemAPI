// ParticleSystem.cpp

#include "ParticleSystem.h"

#include "Settings.h"
#include "Particle.h"
#include "internal\ParticleDefinition.h"
#include "internal\EmitterDefinition.h"
#include "internal\SpawnedParticleContainer.h"
#include "internal\Math.h"

#include <cmath>
#include <algorithm>
#include <random>

#if 0 // InspectBinary
#include <bitset>
#include <string>

namespace util {
	template<typename TInteger>
	std::string InspectBinary(TInteger value) {
		std::bitset<sizeof(TInteger) * 8> bs(value);
		std::string stringValue = bs.to_string();
		stringValue.erase(stringValue.begin(), stringValue.end() - 16);
		return stringValue;
	}
}
#endif

namespace GIMC
{
	/////////////////////////////////////////////////////////////////////
	// ParticleSystem
	/////////////////////////////////////////////////////////////////////

	const unsigned ParticleSystem::MAX_DEFINITIONS = 1000;

	ParticleSystem::ParticleSystem(ParticleSystemSettings& settings)
		: particleDefinitions(nullptr)
	{
		definitionLimit = 
			settings.definitionLimit > 
			MAX_DEFINITIONS ? 
			MAX_DEFINITIONS : 
			settings.definitionLimit;

		emitterLimit = 
			settings.emitterLimitPerDefinition > 
			internal::ParticleDef::MAX_EMITTERS ? 
			internal::ParticleDef::MAX_EMITTERS :
			settings.emitterLimitPerDefinition;

		Reset(settings.particleLimit);
	}

	ParticleSystem::~ParticleSystem()
	{
		CleanUp();
	}

	void ParticleSystem::Update(float deltaTime)
	{
		internal::SpawnedParticleContainer spawnedParticles[MAX_DEFINITIONS];

		for (unsigned i = 0; i < numDefinitions; i++)
		{
			numParticles += particleDefinitions[i].ProcessAll(deltaTime, 
				spawnedParticles[particleDefinitions[i].particle]);
		}

		for (unsigned def = 0; def < numDefinitions; def++)
		{
			for (unsigned i = 0; i < spawnedParticles[def].size; i++)
			{
				particleDefinitions[def].SpawnParticle(spawnedParticles[def].locations[i]);
			}
		}
	}

	Particle ParticleSystem::CreateParticle()
	{
		Particle handle(this);
		handle.valid = (numDefinitions < definitionLimit);
		if (handle.valid)
		{
			handle.uniqueID = numDefinitions;
			particleDefinitions[numDefinitions].Reset();

			numDefinitions++;
		}

		return handle;
	}

	Emitter ParticleSystem::CreateEmitter(Particle spawnedParticle)
	{
		Emitter handle(this);
		handle.particleID = spawnedParticle.uniqueID;

		unsigned currentEmitterCount = particleDefinitions[handle.particleID].numEmitters;
		handle.valid = (spawnedParticle.valid && currentEmitterCount < emitterLimit);
		if (handle.valid)
		{
			handle.uniqueID = currentEmitterCount;
			particleDefinitions[handle.particleID].emitterActive[handle.uniqueID] = true;
			particleDefinitions[handle.particleID].numEmitters++;

			numEmitters++;
		}

		return handle;
	}

	void ParticleSystem::DestroyParticle(Particle& particle)
	{
		if (particle.valid == false)
			return;

		unsigned id = particle.uniqueID;
		particle.valid = false;

		numParticles -= particleDefinitions[id].Reset();

		/*ParticleDef lastElement = particleDefinitions[numDefinitions];
		particleDefinitions[numDefinitions] = particleDefinitions[id];
		particleDefinitions[id] = lastElement;*/

		numDefinitions--;
	}

	void ParticleSystem::DestroyEmitter(Emitter emitter)
	{
		if (emitter.valid == false)
			return;

		emitter.valid = false;
		particleDefinitions[emitter.particleID].emitters[emitter.uniqueID].Reset();
		particleDefinitions[emitter.particleID].emitterActive[emitter.uniqueID] = false;

		numEmitters--;
	}

	void ParticleSystem::SpawnParticle(Particle particle, Vector2 location, unsigned spawnCount)
	{
		if (particle.valid == false)
			return;

		for (unsigned i = 0; i < spawnCount; i++)
		{
			particleDefinitions[particle.uniqueID].SpawnParticle(location);
		}
	}

	void ParticleSystem::ClearVisibleParticles()
	{
		for (unsigned i = 0; i < numDefinitions; i++)
		{
			particleDefinitions[i].numParticles = 0;
		}
		numParticles = 0;
	}

	void ParticleSystem::ClearVisibleParticlesOfType(Particle& particle)
	{
		if (particle.valid == false)
			return;

		internal::ParticleDef& def = particleDefinitions[particle.uniqueID];
		unsigned partCount = def.numParticles;
		def.numParticles = 0;

		numParticles -= partCount;
	}

	unsigned ParticleSystem::GetParticleTypeCount()
	{
		return numDefinitions;
	}

	unsigned ParticleSystem::GetSpawnedParticleCountOfType(unsigned particle)
	{
		return particleDefinitions[particle].numParticles;
	}

	unsigned ParticleSystem::GetSpawnedParticleCount()
	{
		return numParticles;
	}

	unsigned ParticleSystem::GetEmitterTypeCount(unsigned particle)
	{
		return particleDefinitions[particle].numEmitters;
	}

	unsigned ParticleSystem::GetEmitterCount()
	{
		return numEmitters;
	}

	void ParticleSystem::Reset(unsigned newParticleLimit)
	{
		CleanUp();

		internal::Random::setRandomSeed();

		numDefinitions = 0;
		particleDefinitions = new internal::ParticleDef[definitionLimit];

		for (unsigned i = 0; i < definitionLimit; i++)
		{
			particleDefinitions[i].Init(newParticleLimit, emitterLimit);
		}

		numParticles = 0;
		numEmitters = 0;
	}

	// Private

	void ParticleSystem::ParticleSetLifetime(Particle& particle, float minLife, float maxLife)
	{
		if (particle.valid == false)
			return;

		if (minLife == 0.0f && maxLife == 0.0f)
			return;

		internal::ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.minLife = std::min(minLife, maxLife);
		def.maxLife = std::max(minLife, maxLife);
	}

	void ParticleSystem::ParticleSetSize(Particle& particle, float sizeMin, float sizeMax, float sizeChange)
	{
		if (particle.valid == false)
			return;

		internal::ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.sizeMin = std::min(sizeMin, sizeMax);
		def.sizeMax = std::max(sizeMin, sizeMax);
		def.sizeChange = sizeChange;

		if (sizeChange != 0.0f)
		{
			def.AddFlag(internal::Flag_Size);
		}
	}

	void ParticleSystem::ParticleSetRotation(Particle& particle, float rotMin, float rotMax, float rotChange, bool rotRelative)
	{
		if (particle.valid == false)
			return;

		internal::ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.rotationMin = std::min(rotMin, rotMax);
		def.rotationMax = std::max(rotMin, rotMax);
		def.rotationChange = rotChange;
		def.rotationRelative = rotRelative;

		if (rotChange != 0.0f)
		{
			def.AddFlag(internal::Flag_Rotation);
		}
	}

	void ParticleSystem::ParticleSetScale(Particle& particle, float scaleX, float scaleY)
	{
		if (particle.valid == false)
			return;

		internal::ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.scale = Vector2(scaleX, scaleY);
	}

	void ParticleSystem::ParticleSetColor(Particle& particle, Color colorStart, Color colorEnd)
	{
		if (particle.valid == false)
			return;

		internal::ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.colorStart = internal::HSL(colorStart);
		def.colorEnd = internal::HSL(colorEnd);
		def.colorStartAlpha = (float)colorStart.A;
		def.colorEndAlpha = (float)colorEnd.A;

		if (def.colorStart != def.colorEnd)
		{
			if (def.colorStart.Hue != def.colorEnd.Hue)
			{
				// Find shortest degree path between hues
				float diff = fmod(fmod(((float)def.colorEnd.Hue - (float)def.colorStart.Hue), 360.0f) + 540.0f, 360.0f) - 180.0f;
				def.colorDeltaH = diff;

				//def.colorDeltaH = float(def.colorStart.Hue - def.colorEnd.Hue) * -1.0f;
			}

			if (def.colorStart.Saturation != def.colorEnd.Saturation)
				def.colorDeltaS = float(def.colorStart.Saturation - def.colorEnd.Saturation) * -1.0f;
			if (def.colorStart.Luminance != def.colorEnd.Luminance)
				def.colorDeltaL = float(def.colorStart.Luminance - def.colorEnd.Luminance) * -1.0f;

			def.AddFlag(internal::Flag_HSL);
			def.AddFlag(internal::Flag_Color);
		}
		else
		{
			def.RemoveFlag(internal::Flag_HSL);
		}

		if (colorStart.A != colorEnd.A)
		{
			def.colorDeltaA = (def.colorStartAlpha - def.colorEndAlpha) * -1.0f;

			def.AddFlag(internal::Flag_Color);
		}
		else
		{
			def.colorDeltaA = 0.f;
		}
	}

	void ParticleSystem::ParticleSetSpeed(Particle& particle, float speedMin, float speedMax, float speedChange, bool clampToZero)
	{
		if (particle.valid == false)
			return;

		internal::ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.speedMin = std::min(speedMin, speedMax);
		def.speedMax = std::max(speedMin, speedMax);
		def.speedChange = speedChange;
		def.speedClampToZero = clampToZero;

		def.CalcNewVelocityData();
	}

	void ParticleSystem::ParticleSetDirection(Particle& particle, float dirMin, float dirMax, float dirChange)
	{
		if (particle.valid == false)
			return;

		internal::ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.dirMin = std::min(dirMin, dirMax);
		def.dirMax = std::max(dirMin, dirMax);
		def.dirChange = dirChange;

		def.CalcNewVelocityData();
	}

	void ParticleSystem::ParticleSetVelocity(Particle& particle, Vector2 velocity)
	{
		if (particle.valid == false)
			return;

		internal::ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.velocity = velocity;

		if (velocity != Vector2(0.0f, 0.0f))
		{
			def.AddFlag(internal::Flag_Velocity);
			def.AddFlag(internal::Flag_GlobalVelocity);
			def.RemoveFlag(internal::Flag_Speed);
			def.RemoveFlag(internal::Flag_Direction);
		}
	}

	void ParticleSystem::ParticleSetGravity(Particle& particle, float direction, float strength)
	{
		if (particle.valid == false)
			return;

		internal::ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.gravity = Vector2::CreateUnit(direction) * strength;

		if (def.gravity != Vector2(0.0f, 0.0f))
		{
			def.AddFlag(internal::Flag_Gravity);
		}
	}

	void ParticleSystem::ParticleSetAttractorPoint(Particle& particle, Vector2 position, float strength, bool destroyInCenter)
	{
		if (particle.valid == false)
			return;

		internal::ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.attractorPoint = position;
		def.attractorStrength = strength;
		def.attractorKill = destroyInCenter;

		if (strength != 0.0f)
		{
			def.AddFlag(internal::Flag_Attractor);
		}
		else
		{
			def.RemoveFlag(internal::Flag_Attractor);
		}
	}

	void ParticleSystem::ParticleSetAttractorRange(Particle& particle, float range, bool linearfalloff /* = false */)
	{
		if (particle.valid == false)
			return;

		if (range <= 0.0f)
			return;

		internal::ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.attractorRange = range;
		def.attractorLinearFalloff = linearfalloff;
	}

	void ParticleSystem::ParticleSetRotatorPoint(Particle& particle, Vector2 position, bool useDegrees /* = true */)
	{
		if (particle.valid == false)
			return;

		internal::ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.rotatorPoint = position;
		def.rotatorUseDegrees = useDegrees;

		def.AddFlag(internal::Flag_Rotator);
	}

	void ParticleSystem::ParticleSetRotatorRange(Particle& particle, float range, bool useLinearFalloff)
	{
		if (particle.valid == false)
			return;

		if (range <= 0.0f)
			return;

		internal::ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.rotatorRange = range;
		def.rotatorLinearFalloff = useLinearFalloff;
	}

	void ParticleSystem::ParticleSetSpawnedParticle(Particle& particle, Particle spawnedParticle, unsigned numberOfSpawnedParticles)
	{
		if (particle.valid == false)
			return;

		particleDefinitions[particle.uniqueID].particle = spawnedParticle.uniqueID;
		particleDefinitions[particle.uniqueID].particleSpawnCount = numberOfSpawnedParticles;
	}

	void ParticleSystem::ParticleSetCustomData(Particle& particle, void* data)
	{
		if (particle.valid == false)
			return;

		particleDefinitions[particle.uniqueID].customData = data;
	}

	void ParticleSystem::ParticleReset(Particle& particle)
	{
		if (particle.valid == false)
			return;

		particleDefinitions[particle.uniqueID].Reset();
	}

	void ParticleSystem::EmitterBurst(Emitter emitter, unsigned spawnedParticlesOverride)
	{
		if (emitter.valid == false)
			return;

		numParticles += particleDefinitions[emitter.particleID].Burst
			(emitter.uniqueID, spawnedParticlesOverride);
	}

	void ParticleSystem::EmitterSetLocation(Emitter emitter, Vector2 location)
	{
		if (emitter.valid == false)
			return;

		internal::EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];
		def.location = location;
	}

	void ParticleSystem::EmitterSetPoint(Emitter emitter, Vector2 location)
	{
		if (emitter.valid == false)
			return;

		internal::EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];
		def.shape = EmitterShape::POINT;
		def.location = location;
	}

	void ParticleSystem::EmitterSetCircle(Emitter emitter, float radius, Vector2 location)
	{
		if (emitter.valid == false)
			return;

		internal::EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];
		def.shape = EmitterShape::CIRCLE;
		def.location = location;
		def.dimension = Vector2(radius, radius);
	}

	void ParticleSystem::EmitterSetRectangle(Emitter emitter, Vector2 dimension, Vector2 location)
	{
		if (emitter.valid == false)
			return;

		internal::EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];

		def.shape = EmitterShape::RECTANGLE;
		def.location = location;
		def.dimension = dimension;

		def.CalcRectRim();
	}

	void ParticleSystem::EmitterSetRim(Emitter emitter, float thickness)
	{
		if (emitter.valid == false)
			return;

		internal::EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];

		def.rim = thickness;

		def.CalcRectRim();
	}

	void ParticleSystem::EmitterSetFrequency(Emitter emitter, float frequency, unsigned spawnCount, bool spawnImmediately)
	{
		if (emitter.valid == false)
			return;

		internal::EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];
		def.frequency = frequency;
		def.particleCount = spawnCount;
		if (spawnImmediately)
		{
			def.InitTimer();
		}
	}

	void ParticleSystem::EmitterSetActive(Emitter emitter, bool state)
	{
		internal::ParticleDef& def = particleDefinitions[emitter.particleID];

		def.emitterActive[emitter.uniqueID] = state;
	}

	internal::ParticleDef* ParticleSystem::getDefinitionFromIndexParticles(unsigned& index)
	{
		if (numParticles == 0)
			return nullptr;

		for (unsigned i = index; i < numDefinitions; i++)
		{
			if (particleDefinitions[i].numParticles > 0)
			{
				index = i;
				return &particleDefinitions[i];
			}
		}

		return nullptr;
	}

	internal::ParticleDef* ParticleSystem::getDefenitionFromIndexEmitters(unsigned& index)
	{
		for (unsigned i = index; i < numDefinitions; i++)
		{
			if (particleDefinitions[i].numEmitters > 0)
			{
				index = i;
				return &particleDefinitions[i];
			}
		}

		return nullptr;
	}

	ParticleOutput* ParticleSystem::GetParticle(unsigned particleIndex)
	{
		if (particleIndex > particleDefinitions[0].numParticles)
			return &particleDefinitions[0].particles[particleDefinitions[0].numParticles];

		return &particleDefinitions[0].particles[particleIndex];
	}

	void ParticleSystem::CleanUp()
	{
		if (particleDefinitions != nullptr)
		{
			delete[] particleDefinitions;
			particleDefinitions = nullptr;
		}
	}
};