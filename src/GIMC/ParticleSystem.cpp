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

	void ParticleSystem::ParticleSetLifetime(unsigned particleID, float minLife, float maxLife)
	{
		if (minLife == 0.0f && maxLife == 0.0f)
			return;

		internal::ParticleDef& def = particleDefinitions[particleID];
		def.minLife = std::min(minLife, maxLife);
		def.maxLife = std::max(minLife, maxLife);
	}

	void ParticleSystem::ParticleSetSize(unsigned particleID, float sizeMin, float sizeMax, float sizeChange)
	{
		internal::ParticleDef& def = particleDefinitions[particleID];
		def.sizeMin = std::min(sizeMin, sizeMax);
		def.sizeMax = std::max(sizeMin, sizeMax);
		def.sizeChange = sizeChange;

		if (sizeChange != 0.0f)
		{
			def.AddFlag(internal::Flag_Size);
		}
	}

	void ParticleSystem::ParticleSetRotation(unsigned particleID, float rotMin, float rotMax, float rotChange, bool rotRelative)
	{
		internal::ParticleDef& def = particleDefinitions[particleID];
		def.rotationMin = std::min(rotMin, rotMax);
		def.rotationMax = std::max(rotMin, rotMax);
		def.rotationChange = rotChange;
		def.rotationRelative = rotRelative;

		if (rotChange != 0.0f)
		{
			def.AddFlag(internal::Flag_Rotation);
		}
	}

	void ParticleSystem::ParticleSetScale(unsigned particleID, float scaleX, float scaleY)
	{
		internal::ParticleDef& def = particleDefinitions[particleID];
		def.scale = Vector2(scaleX, scaleY);
	}

	void ParticleSystem::ParticleSetColor(unsigned particleID, Color colorStart, Color colorEnd)
	{
		internal::ParticleDef& def = particleDefinitions[particleID];
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

	void ParticleSystem::ParticleSetSpeed(unsigned particleID, float speedMin, float speedMax, float speedChange, bool clampToZero)
	{
		internal::ParticleDef& def = particleDefinitions[particleID];
		def.speedMin = std::min(speedMin, speedMax);
		def.speedMax = std::max(speedMin, speedMax);
		def.speedChange = speedChange;
		def.speedClampToZero = clampToZero;

		def.CalcNewVelocityData();
	}

	void ParticleSystem::ParticleSetDirection(unsigned particleID, float dirMin, float dirMax, float dirChange)
	{
		internal::ParticleDef& def = particleDefinitions[particleID];
		def.dirMin = std::min(dirMin, dirMax);
		def.dirMax = std::max(dirMin, dirMax);
		def.dirChange = dirChange;

		def.CalcNewVelocityData();
	}

	void ParticleSystem::ParticleSetVelocity(unsigned particleID, Vector2 velocity)
	{
		internal::ParticleDef& def = particleDefinitions[particleID];
		def.velocity = velocity;

		if (velocity != Vector2(0.0f, 0.0f))
		{
			def.AddFlag(internal::Flag_Velocity);
			def.AddFlag(internal::Flag_GlobalVelocity);
			def.RemoveFlag(internal::Flag_Speed);
			def.RemoveFlag(internal::Flag_Direction);
		}
	}

	void ParticleSystem::ParticleSetGravity(unsigned particleID, float direction, float strength)
	{
		internal::ParticleDef& def = particleDefinitions[particleID];
		def.gravity = Vector2::CreateUnit(direction) * strength;

		if (def.gravity != Vector2(0.0f, 0.0f))
		{
			def.AddFlag(internal::Flag_Gravity);
		}
	}

	void ParticleSystem::ParticleSetAttractorPoint(unsigned particleID, Vector2 position, float strength, bool destroyInCenter)
	{
		internal::ParticleDef& def = particleDefinitions[particleID];
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

	void ParticleSystem::ParticleSetAttractorRange(unsigned particleID, float range, bool linearfalloff /* = false */)
	{
		if (range <= 0.0f)
			return;

		internal::ParticleDef& def = particleDefinitions[particleID];
		def.attractorRange = range;
		def.attractorLinearFalloff = linearfalloff;
	}

	void ParticleSystem::ParticleSetRotatorPoint(unsigned particleID, Vector2 position, bool useDegrees /* = true */)
	{
		internal::ParticleDef& def = particleDefinitions[particleID];
		def.rotatorPoint = position;
		def.rotatorUseDegrees = useDegrees;

		def.AddFlag(internal::Flag_Rotator);
	}

	void ParticleSystem::ParticleSetRotatorRange(unsigned particleID, float range, bool useLinearFalloff)
	{
		if (range <= 0.0f)
			return;

		internal::ParticleDef& def = particleDefinitions[particleID];
		def.rotatorRange = range;
		def.rotatorLinearFalloff = useLinearFalloff;
	}

	void ParticleSystem::ParticleSetSpawnedParticle(unsigned particleID, unsigned spawnedParticleID, unsigned numberOfSpawnedParticles)
	{
		particleDefinitions[particleID].particle = spawnedParticleID;
		particleDefinitions[particleID].particleSpawnCount = numberOfSpawnedParticles;
	}

	void ParticleSystem::ParticleSetCustomData(unsigned particleID, void* data)
	{
		particleDefinitions[particleID].customData = data;
	}

	void ParticleSystem::ParticleReset(unsigned particleID)
	{
		particleDefinitions[particleID].Reset();
	}

	void ParticleSystem::EmitterBurst(unsigned emitterID, unsigned particleID, unsigned spawnedParticlesOverride)
	{
		numParticles += particleDefinitions[particleID].Burst
			(emitterID, spawnedParticlesOverride);
	}

	void ParticleSystem::EmitterSetFrequency(unsigned emitterID, unsigned particleID, float frequency, unsigned spawnCount, bool spawnImmediately)
	{
		internal::EmitterDef& def = particleDefinitions[particleID].emitters[emitterID];
		def.frequency = frequency;
		def.particleCount = spawnCount;
		if (spawnImmediately)
		{
			def.InitTimer();
		}
	}

	void ParticleSystem::EmitterSetLocation(unsigned emitterID, unsigned particleID, Vector2 location)
	{
		internal::EmitterDef& def = particleDefinitions[particleID].emitters[emitterID];
		def.location = location;
	}

	void ParticleSystem::EmitterSetPoint(unsigned emitterID, unsigned particleID, Vector2 location)
	{
		internal::EmitterDef& def = particleDefinitions[particleID].emitters[emitterID];
		def.shape = EmitterShape::POINT;
		def.location = location;
	}

	void ParticleSystem::EmitterSetCircle(unsigned emitterID, unsigned particleID, float radius, Vector2 location)
	{
		internal::EmitterDef& def = particleDefinitions[particleID].emitters[emitterID];
		def.shape = EmitterShape::CIRCLE;
		def.location = location;
		def.dimension = Vector2(radius, radius);
	}

	void ParticleSystem::EmitterSetRectangle(unsigned emitterID, unsigned particleID, Vector2 dimension, Vector2 location)
	{
		internal::EmitterDef& def = particleDefinitions[particleID].emitters[emitterID];

		def.shape = EmitterShape::RECTANGLE;
		def.location = location;
		def.dimension = dimension;

		def.CalcRectRim();
	}

	void ParticleSystem::EmitterSetRim(unsigned emitterID, unsigned particleID, float thickness)
	{
		internal::EmitterDef& def = particleDefinitions[particleID].emitters[emitterID];

		def.rim = thickness;

		def.CalcRectRim();
	}

	void ParticleSystem::EmitterSetActive(unsigned emitterID, unsigned particleID, bool state)
	{
		internal::ParticleDef& def = particleDefinitions[particleID];

		def.emitterActive[emitterID] = state;
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