// ParticleDefinition.cpp

#include "ParticleDefinition.h"

#include "..\Settings.h"
#include "..\Output.h"
#include "..\debug\EmitterDebug.h"
#include "Math.h"

namespace PS
{
	namespace internal
	{
		/////////////////////////////////////////////////////////////////////
		// ParticleDef
		/////////////////////////////////////////////////////////////////////

		ParticleDef::ParticleDef()
		{
			particles = new ParticleOutput[MAX_PARTICLES];
			spawnedParticles = new Vector2[MAX_PARTICLES];
			emitters = new EmitterDef[MAX_EMITTERS];
			emitterActive = new bool[MAX_EMITTERS];

			Reset();
		}

		ParticleDef::~ParticleDef()
		{
			delete[] particles;
			particles = nullptr;

			delete[] spawnedParticles;
			spawnedParticles = nullptr;

			delete[] emitters;
			emitters = nullptr;

			delete[] emitterActive;
			emitterActive = nullptr;
		}

		unsigned ParticleDef::Reset()
		{
			for (unsigned i = 0; i < MAX_PARTICLES; i++)
			{
				particles[i] = ParticleOutput();
				spawnedParticles[i] = Vector2();
			}

			for (unsigned i = 0; i < MAX_EMITTERS; i++)
			{
				emitters[i] = EmitterDef();
				emitterActive[i] = true;
			}

			minLife = 1.f;
			maxLife = 1.0f;

			scale = Vector2(1.0f, 1.0f);

			// Size
			sizeMin = 32.0f;
			sizeMax = 32.0f;
			sizeChange = 0.0f;

			// Rotation
			rotationRelative = false;
			rotationMin = 0.0f;
			rotationMax = 0.0f;
			rotationChange = 0.0f;

			// Color
			colorDeltaH = 0.0f;
			colorDeltaS = 0.0f;
			colorDeltaL = 0.0f;
			colorDeltaA = 0.0f;
			colorStart = Color();
			colorEnd = Color();

			// Speed
			speedMin = 0.0f;
			speedMax = 0.0f;
			speedChange = 0.0f;
			speedClampToZero = false;

			// Direction
			dirMin = 0.0f;
			dirMax = 0.0f;
			dirChange = 0.0f;

			velocity = Vector2(0.0f, 0.0f);
			gravity = Vector2(0.0f, 0.0f);

			// Attractor
			attractorPoint = Vector2(0.0f, 0.0f);
			attractorStrength = 0.0f;
			attractorKill = false;
			attractorRange = FLT_MAX;
			attractorLinearFalloff = false;

			// Rotator
			rotatorPoint = Vector2(0.0f, 0.0f);
			rotatorUseDegrees = true;
			rotatorRange = FLT_MAX;
			rotatorLinearFalloff = false;

			particle = (unsigned)-1;
			particleSpawnCount = 0;

			customData = nullptr;
			flagBits = 0x0000;

			unsigned ParticlesLeftBeforeReset = numParticles;

			numParticles = 0;
			numSpawnedParticles = 0;
			numEmitters = 0;

			return(ParticlesLeftBeforeReset);
		}

		int ParticleDef::ProcessAll(float deltaTime, SpawnedParticleContainer& container)
		{
			int Result = 0;

			if (deltaTime == 0.0f)
				return Result;

			// Particles
			for (unsigned i = 0; i < numParticles; i++)
			{
				particles[i].lifeRemaining -= deltaTime;
				if (particles[i].lifeRemaining <= 0.0f)
				{
					if (particle != (unsigned)-1)
					{
						for (unsigned j = 0; j < particleSpawnCount; j++)
						{
							container.Add(particles[i].location);
						}
					}

					removeParticle(i);
					Result--;

					i--;
					continue;
				}

				Process(particles[i], deltaTime);
			}

			// Emitters
			for (unsigned i = 0; i < numEmitters; i++)
			{
				if (emitterActive[i] == false)
					continue;

				unsigned framesPassed = emitters[i].Update(deltaTime);
				if (framesPassed > 0)
				{
					unsigned particleCount = emitters[i].particleCount * framesPassed;
					for (unsigned j = 0; j < particleCount; j++)
					{
						SpawnParticle(emitters[i].GetSpawnLocation());
					}
				}
			}

			// SpawnedParticles
			for (unsigned i = 0; i < numSpawnedParticles; i++)
			{
				if (addParticle(spawnedParticles[i]))
				{
					Result++;
				}
				else
				{
					break;
				}
			}
			numSpawnedParticles = 0;

			return Result;
		}

		unsigned ParticleDef::Burst(unsigned emitterIndex, unsigned spawnedParticlesOverride)
		{
			unsigned Result = 0;

			unsigned numSpawnedBurstParts = 0;
			Vector2 numBursted[MAX_PARTICLES];

			unsigned particleCount =
				spawnedParticlesOverride == (unsigned)-1 ? emitters[emitterIndex].particleCount
				: spawnedParticlesOverride;
			for (unsigned j = 0; j < particleCount; j++)
			{
				numBursted[numSpawnedBurstParts++] = emitters[emitterIndex].GetSpawnLocation();
			}

			for (unsigned i = 0; i < numSpawnedBurstParts; i++)
			{
				if (addParticle(numBursted[i]))
				{
					Result++;
				}
				else
				{
					break;
				}
			}

			return Result;
		}

		void ParticleDef::SpawnParticle(Vector2 location)
		{
			if (numSpawnedParticles < MAX_PARTICLES)
			{
				spawnedParticles[numSpawnedParticles] = location;
				numSpawnedParticles++;
			}
		}

		unsigned ParticleDef::GetParticleCount()
		{
			return numParticles;
		}

		ParticleOutput* ParticleDef::GetParticle(unsigned particleIndex)
		{
			if (particleIndex > numParticles)
				return &particles[0];

			return &particles[particleIndex];
		}

		unsigned ParticleDef::GetEmitterCount()
		{
			return numEmitters;
		}

		EmitterDebugOutput ParticleDef::GetEmitter(unsigned emitterIndex)
		{
			EmitterDef* emitter = &emitters[emitterIndex];

			EmitterDebugOutput output;
			output.location = emitter->location;
			output.dims = emitter->dimension;
			output.shape = emitter->shape;
			output.rim = emitter->rim;
			output.active = emitterActive[emitterIndex];

			return output;
		}

		void ParticleDef::AddFlag(EParticleFlags flag)
		{
			flagBits |= flag;
		}

		void ParticleDef::RemoveFlag(EParticleFlags flag)
		{
			flagBits &= ~flag;
		}

		bool ParticleDef::HasFlag(EParticleFlags flag)
		{
			bool Result = (flagBits & flag ? true : false);
			return(Result);
		}

		void ParticleDef::CalcNewVelocityData()
		{
			bool constSpeed = false;
			bool constDirection = false;
			bool varyingSpeed = false;
			bool varyingDirection = false;

			if (speedChange != 0.0f) // dynamic speed
			{
				AddFlag(Flag_Speed);
				varyingSpeed = true;
			}
			else // no dynamic speed
			{
				if (speedMin == speedMax)
				{
					if (speedMax > 0.0f) // Has any speed
					{
						constSpeed = true;
					}
					else
					{
						// Still. No velocity
						RemoveFlag(Flag_Velocity);
						RemoveFlag(Flag_GlobalVelocity);
						return;
					}
				}
				RemoveFlag(Flag_Speed);
			}

			AddFlag(Flag_Velocity);

			if (dirChange != 0.0f) // dynamic direction
			{
				AddFlag(Flag_Direction);
				varyingDirection = true;
			}
			else // No dynamic direction
			{
				if (dirMin == dirMax)
				{
					if (dirMax != 0.0f) // Has constant direction
					{
						constDirection = true;
					}
				}
				RemoveFlag(Flag_Direction);
			}

			if (constSpeed && constDirection) // Constant velocity
			{
				AddFlag(Flag_GlobalVelocity);
				velocity = Vector2::CreateUnit(dirMax) * speedMax;
				return;
			}
			else
			{
				RemoveFlag(Flag_GlobalVelocity);
			}

			if (varyingDirection == false && varyingSpeed == false)
			{
				AddFlag(Flag_ConstVelocity);
			}
			else
			{
				RemoveFlag(Flag_ConstVelocity);
			}
		}

		// Private

		void ParticleDef::Process(ParticleOutput& output, float deltaTime)
		{
			//Size
			if (HasFlag(Flag_Size))
			{
				output.size = updateSize(output.size, deltaTime);
			}

			// Color
			if (HasFlag(Flag_Color))
				updateColor(output, deltaTime);

			bool successfulRotation = false;
			bool hasAlreadyUpdatedSpeed = false;

			if (HasFlag(Flag_Rotator))
			{
				if (HasFlag(Flag_Speed)) // Rotation speed
				{
					output.locationData.speed()
						= updateSpeed(output.locationData.speed(), deltaTime);

					hasAlreadyUpdatedSpeed = true;
				}

				successfulRotation = updateRotator(output, deltaTime);
			}

			if (!successfulRotation && HasFlag(Flag_Velocity)) // Velocity
			{
				if (HasFlag(Flag_ConstVelocity))
				{
					output.location += output.locationData.Velocity * deltaTime;
				}
				else
				{
					bool shouldUpdateSpeed = HasFlag(Flag_Speed) && !hasAlreadyUpdatedSpeed;
					bool shouldUpdateDirection = HasFlag(Flag_Direction);

					ParticleOutput::LocationData locationData;
					locationData = output.locationData;
					float currentSpeed = locationData.speed();
					float currentDirection = locationData.direction();

					// Dynamic Speed
					if (shouldUpdateSpeed)
					{
						currentSpeed = updateSpeed(currentSpeed, deltaTime);
					}

					// Dynamic Direction
					if (shouldUpdateDirection)
					{
						currentDirection = updateDirection(currentDirection, deltaTime);
					}

					// Return calculated data to subject
					locationData.speed() = currentSpeed;
					locationData.direction() = currentDirection;
					output.locationData = locationData;

					// Location
					output.location +=
						Vector2::CreateUnit(currentDirection) * currentSpeed * deltaTime;
				}
			}

			if (HasFlag(Flag_Gravity))
				output.location += gravity * deltaTime;

			if (HasFlag(Flag_Attractor))
				updateAttractor(output, deltaTime);

			// Rotation
			if (HasFlag(Flag_Rotation))
				updateRotation(output, deltaTime);
		}

		float ParticleDef::updateSize(float currentSize, float deltaTime)
		{
			float Result = currentSize + sizeChange*deltaTime;

			if (Result < 0.0f)
			{
				Result = 0.0f;
			}

			return(Result);
		}

		void ParticleDef::updateRotation(ParticleOutput& output, float deltaTime)
		{
			if (rotationRelative)
				output.rotation = output.locationData.direction();
			else
			{
				output.rotation += rotationChange * deltaTime;
			}
		}

		void ParticleDef::updateColor(ParticleOutput& output, float deltaTime)
		{
			ParticleOutput::ColorData data = output.colorData;
			Color Result = output.color;
			float colorH = data.H;
			float colorS = data.S;
			float colorL = data.L;
			float colorA = data.A;

			// Alpha
			colorA += colorDeltaA * deltaTime * data.deltaFactor;
			if (colorA > 255.0f) colorA = 255.0f;
			if (colorA < 0.0f)   colorA = 0.0f;
			data.A = colorA;

			// HSL
			if (HasFlag(Flag_HSL))
			{
				colorH += colorDeltaH * deltaTime * data.deltaFactor;
				colorS += colorDeltaS * deltaTime * data.deltaFactor;
				colorL += colorDeltaL * deltaTime * data.deltaFactor;

				// Wrap & clamp values
				if (colorH > 360.0f) colorH = 0.0f;
				if (colorH < 0.0f)   colorH = 360.0f;
				if (colorS > 100.0f) colorS = 100.0f;
				if (colorS < 0.0f)   colorS = 0.0f;
				if (colorL > 100.0f) colorL = 100.0f;
				if (colorL < 0.0f)   colorL = 0.0f;

				data.H = colorH;
				data.S = colorS;
				data.L = colorL;

				Result = HSL((int)colorH, (int)colorS, (int)colorL).TurnToRGB();
			}

			Result.A = (unsigned char)colorA;

			output.color = Result;
			output.colorData = data;
		}

		float ParticleDef::updateSpeed(float currentSpeed, float deltaTime)
		{
			float Result = currentSpeed + (speedChange * deltaTime);

			if (speedClampToZero && Result < 0.0f)
				Result = 0.0f;

			return(Result);
		}

		float ParticleDef::updateDirection(float currentDirection, float deltaTime)
		{
			float Result = currentDirection + (dirChange * deltaTime);
			return(Result);
		}

		void ParticleDef::updateAttractor(ParticleOutput& output, float deltaTime)
		{
			float radius = output.location.Distance(attractorPoint);

			if (radius > attractorRange) // Out of range
				return;

			float effect = attractorLinearFalloff ? (1.0f - (radius / attractorRange)) : 1.0f;

			Vector2 currentLocation = output.location;
			Vector2 difference = attractorPoint - currentLocation;
			if (attractorKill)
			{
				if (difference.Length() < 1.0f)
				{
					output.lifeRemaining = 0.0f;
				}
			}
			difference.Normalize();

			output.location += difference * attractorStrength * deltaTime * effect;
		}

		bool ParticleDef::updateRotator(ParticleOutput& output, float deltaTime)
		{
			float radius = output.location.Distance(rotatorPoint);

			if (radius > rotatorRange) // Out of range
				return false;

			float effect = rotatorLinearFalloff ? (1.0f - (radius / rotatorRange)) : 1.0f;

			float angle = atan2(
				output.location.Y - rotatorPoint.Y,
				output.location.X - rotatorPoint.X);

			angle *= Math::radToDeg;

			if (rotatorUseDegrees)
			{
				angle += output.locationData.speed() * deltaTime * effect;
			}
			else
			{
				float circumference = radius * Math::PI2;
				float oneDegreeLength = circumference / 360.0f;

				float rimLocation = oneDegreeLength * angle;
				float addition = output.locationData.speed() * deltaTime * effect;
				rimLocation += addition;

				if (rimLocation < 0)
					rimLocation = circumference + rimLocation;
				if (rimLocation >= circumference)
					rimLocation -= circumference;

				angle = rimLocation / oneDegreeLength;
			}

			output.location = rotatorPoint + Vector2::CreateUnit(angle) * radius;

			return true;
		}

		bool ParticleDef::addParticle(Vector2 location)
		{
			if (numParticles >= MAX_PARTICLES)
				return false;

			unsigned slot = numParticles;
			initParticle(particles[slot]);
			particles[slot].location = location;

			numParticles++;

			return true;
		}

		bool ParticleDef::removeParticle(unsigned particleIndex)
		{
			if (particleIndex > numParticles)
				return false;

			ParticleOutput lastElement = particles[numParticles];
			particles[numParticles] = particles[particleIndex];
			particles[particleIndex] = lastElement;

			numParticles--;

			return true;
		}

		void ParticleDef::initParticle(ParticleOutput& output)
		{
			output.lifeRemaining = Random::betweenf(minLife, maxLife);
			output.size = Random::betweenf(sizeMin, sizeMax);
			output.rotation = Random::betweenf(rotationMin, rotationMax);
			output.scale = scale;

			output.colorData.Set(colorStart, colorStartAlpha);
			output.colorData.deltaFactor = (1.0f / output.lifeRemaining);
			output.color = colorStart.TurnToRGB();
			output.color.A = (unsigned char)colorStartAlpha;

			output.customData = customData;

			// Constant velocity
			if (HasFlag(Flag_GlobalVelocity))
			{
				output.locationData.Velocity = velocity;
				return;
			}

			output.locationData.direction() = Random::betweenf(dirMin, dirMax);
			output.locationData.speed() = Random::betweenf(speedMin, speedMax);

			if (HasFlag(Flag_ConstVelocity)) // Individual
			{
				output.locationData.Velocity =
					Vector2::CreateUnit(output.locationData.direction()) * output.locationData.speed();
			}
		}
	};
};