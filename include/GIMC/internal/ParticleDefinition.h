// ParticleDefinition.h

#pragma once

#include "debugging\EmitterDebug.h"

#include "SpawnedParticleContainer.h"
#include "EmitterDefinition.h"
#include "Output.h"

namespace GIMC
{
	namespace internal
	{
		enum EParticleFlags
		{
			Flag_Velocity		= 0x0001, // 1
			Flag_Speed			= 0x0002, // 2
			Flag_Direction		= 0x0004, // 3
			Flag_ConstVelocity	= 0x0008, // 4
			Flag_Size			= 0x0010, // 5
			Flag_Rotation		= 0x0020, // 6
			Flag_Color			= 0x0040, // 7
			Flag_HSL			= 0x0080, // 8
			Flag_GlobalVelocity	= 0x0100, // 9
			Flag_Gravity		= 0x0200, // 10
			Flag_Attractor		= 0x0400, // 11
			Flag_Rotator		= 0x0800, // 12
			Flag_Flag13			= 0x1000, // 13
			Flag_Flag14			= 0x2000, // 14
			Flag_Flag15			= 0x4000, // 15
			Flag_Flag16			= 0x8000  // 16
		};

		class ParticleDef
		{
			friend class ParticleSystem;

		public:

			unsigned Reset();
			int ProcessAll(float deltaTime, SpawnedParticleContainer& container);

			unsigned Burst(unsigned emitterIndex, unsigned spawnedParticlesOverride = (unsigned)-1);
			void SpawnParticle(Vector2 location);

			unsigned GetParticleCount();
			ParticleOutput* GetParticle(unsigned particleindex);
			unsigned GetEmitterCount();
			EmitterDebugOutput GetEmitter(unsigned emitterIndex);

			void AddFlag(EParticleFlags flag);
			void RemoveFlag(EParticleFlags flag);
			bool HasFlag(EParticleFlags flag);

			void CalcNewVelocityData();

		private:

			void Init(unsigned newParticleLimit, unsigned newEmitterLimit);
			ParticleDef() {};
			~ParticleDef();

		public:

			float minLife, maxLife;

			Vector2 scale;

			// Size
			float sizeMin, sizeMax;
			float sizeChange;

			// Rotation
			bool rotationRelative;
			float rotationMin, rotationMax;
			float rotationChange;

			// Color
			float colorDeltaH, colorDeltaS, colorDeltaL, colorDeltaA;
			float colorStartAlpha, colorEndAlpha;
			HSL colorStart, colorEnd;

			// Speed
			float speedMin, speedMax;
			float speedChange;
			bool speedClampToZero;

			// Direction
			float dirMin, dirMax;
			float dirChange;

			Vector2 velocity;
			Vector2 gravity;

			// Attractor
			Vector2 attractorPoint;
			float attractorStrength;
			bool attractorKill;
			float attractorRange;
			bool attractorLinearFalloff;

			// Rotator
			Vector2 rotatorPoint;
			bool rotatorUseDegrees;
			float rotatorRange;
			bool rotatorLinearFalloff;

			unsigned particle;
			unsigned particleSpawnCount;
			void* customData;

		private:

			void Process(ParticleOutput& output, float deltaTime);

			float updateSize(float currentSize, float deltaTime);
			void updateRotation(ParticleOutput& output, float deltaTime);

			void updateColor(ParticleOutput& output, float deltaTime);

			float updateSpeed(float currentSpeed, float deltaTime);
			float updateDirection(float currentDirection, float deltaTime);

			void updateAttractor(ParticleOutput& output, float deltaTime);
			bool updateRotator(ParticleOutput& output, float deltaTime);

			bool addParticle(Vector2 location);
			bool removeParticle(unsigned particleIndex);

			void initParticle(ParticleOutput& info);

		private:

			unsigned short flagBits;

			unsigned numParticles;
			unsigned particleLimit;
			ParticleOutput* particles;
			unsigned numSpawnedParticles;
			Vector2* spawnedParticles;

			unsigned numEmitters;
			unsigned emitterLimit;
			EmitterDef* emitters;
			bool* emitterActive;

			static const unsigned MAX_PARTICLES;
			static const unsigned MAX_EMITTERS;
		};
	};
};