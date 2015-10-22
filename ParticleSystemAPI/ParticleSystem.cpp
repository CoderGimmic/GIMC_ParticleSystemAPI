// ParticleSystem.cpp

#include "ParticleSystem.h"

#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>

#include <stdlib.h>

#if 0
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

namespace PS
{
	/////////////////////////////////////////////////////////////////////
	// Vector2
	/////////////////////////////////////////////////////////////////////

	Vector2::Vector2()
		: X(0.0f)
		, Y(0.0f)
	{}

	Vector2::Vector2(float x, float y)
		: X(x)
		, Y(y)
	{}

	Vector2& Vector2::operator=(const Vector2& rhs)
	{
		X = rhs.X;
		Y = rhs.Y;
		return *this;
	}

	Vector2& Vector2::operator+=(const Vector2& rhs)
	{
		X += rhs.X;
		Y += rhs.Y;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& rhs)
	{
		X -= rhs.X;
		Y -= rhs.Y;
		return *this;
	}

	Vector2& Vector2::operator*=(const float value)
	{
		X *= value;
		Y *= value;
		return *this;
	}

	Vector2& Vector2::operator/=(const float value)
	{
		X /= value;
		Y /= value;
		return *this;
	}

	Vector2 Vector2::operator+(const Vector2& rhs)
	{
		return Vector2(X + rhs.X, Y + rhs.Y);
	}

	Vector2 Vector2::operator-(const Vector2& rhs)
	{
		return Vector2(X - rhs.X, Y - rhs.Y);
	}

	Vector2 Vector2::operator*(const Vector2& rhs)
	{
		return Vector2(X * rhs.X, Y * rhs.Y);
	}

	Vector2 Vector2::operator/(const Vector2& rhs)
	{
		return Vector2(X / rhs.X, Y / rhs.Y);
	}

	Vector2 Vector2::operator*(float value)
	{
		return Vector2(X * value, Y * value);
	}

	bool operator!=(Vector2& first, Vector2& second)
	{
		return (first.X != second.X ||
				first.Y != second.Y);
	}

	float Vector2::length()
	{
		return std::sqrt(X * X + Y * Y);
	}

	void Vector2::normalize()
	{
		float len = length();
		if (len > 0.0f)
		{
			float inverted = 1.0f / len;
			X *= inverted;
			Y *= inverted;
		}
	}

	/////////////////////////////////////////////////////////////////////
	// Color
	/////////////////////////////////////////////////////////////////////

	Color::Color()
		: R(255)
		, G(255)
		, B(255)
		, A(255)
	{}

	Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		: R(r)
		, G(g)
		, B(b)
		, A(a)
	{
		clampColor();
	}

	bool operator!=(Color& first, Color& second)
	{
		return (first.R != second.R ||
				first.G != second.G ||
				first.B != second.B ||
				first.A != second.A);
	}

	// Private

	void Color::clampColor()
	{
		R = clampValue(R);
		G = clampValue(G);
		B = clampValue(B);
		A = clampValue(A);
	}

	unsigned char Color::clampValue(unsigned char value)
	{
		if (value > 255)
			return 255;

		if (value < 0)
			return 0;

		return value;
	}

	/////////////////////////////////////////////////////////////////////
	// HSL
	/////////////////////////////////////////////////////////////////////

	double HSL::EPSILON = 0.00000000000001;

	HSL::HSL()
		: Hue(0)
		, Saturation(0)
		, Luminance(0)
	{}

	HSL::HSL(const Color& C)
	{
		double R, G, B;
		R = C.R / 255.0;
		G = C.G / 255.0;
		B = C.B / 255.0;

		double max, min, l, s = 0.f;

		max = std::max(std::max(R, G), B);

		min = std::min(std::min(R, G), B);

		l = ((max + min) / 2.0);

		if (max - min <= HSL::EPSILON)
		{
			Hue = 0;
			Saturation = 0;
		}
		else
		{
			double diff = max - min;

			if (Luminance < 0.5)
			{
				s = diff / (max + min);
			}
			else
			{
				s = diff / (2 - max - min);
			}

			double diffR = (((max - R) * 60) + (diff / 2.0)) / diff;
			double diffG = (((max - G) * 60) + (diff / 2.0)) / diff;
			double diffB = (((max - B) * 60) + (diff / 2.0)) / diff;


			if (max - R <= HSL::EPSILON) { Hue = diffB - diffG; }
			else if (max - G <= HSL::EPSILON) { Hue = (1 * 360) / 3.0 + (diffR - diffB); }
			else if (max - B <= HSL::EPSILON) { Hue = (2 * 360) / 3.0 + (diffG - diffR); }

			if (Hue <= 0 || Hue >= 360) { fmod(Hue, 360); }

			s *= 100;
		}

		l *= 100;
		Saturation = s;
		Luminance = l;
	}

	HSL::HSL(int H, int S, int L)
	{
		///Range control for Hue.
		if (H <= 360 && H >= 0) { Hue = H; }
		else
		{
			if (H > 360) { Hue = H % 360; }
			else if (H < 0 && H > -360) { Hue = -H; }
			else if (H < 0 && H < -360) { Hue = -(H % 360); }
		}

		///Range control for Saturation.
		if (S <= 100 && S >= 0) { Saturation = S; }
		else
		{
			if (S > 100) { Saturation = S % 100; }
			else if (S < 0 && S > -100) { Saturation = -S; }
			else if (S < 0 && S < -100) { Saturation = -(S % 100); }
		}

		///Range control for Luminance
		if (L <= 100 && L >= 0) { Luminance = L; }
		else
		{
			if (L > 100) { Luminance = L % 100; }
			if (L < 0 && L > -100) { Luminance = -L; }
			if (L < 0 && L < -100) { Luminance = -(L % 100); }
		}

	}

	double HSL::HueToRGB(double arg1, double arg2, double H)
	{
		if (H < 0) H += 1;
		if (H > 1) H -= 1;
		if ((6 * H) < 1) { return (arg1 + (arg2 - arg1) * 6 * H); }
		if ((2 * H) < 1) { return arg2; }
		if ((3 * H) < 2) { return (arg1 + (arg2 - arg1) * ((2.0 / 3.0) - H) * 6); }
		return arg1;
	}

	Color HSL::TurnToRGB()
	{
		///Reconvert to range [0,1]
		double H = Hue / 360.0;
		double S = Saturation / 100.0;
		double L = Luminance / 100.0;

		double arg1, arg2;

		if (S <= EPSILON)
		{
			Color C((unsigned char)L * 255, (unsigned char)L * 255, (unsigned char)L * 255);
			return C;
		}
		else
		{
			if (L < 0.5) { arg2 = L * (1 + S); }
			else { arg2 = (L + S) - (S * L); }
			arg1 = 2 * L - arg2;

			double r = (255 * HueToRGB(arg1, arg2, (H + 1.0 / 3.0)));
			double g = (255 * HueToRGB(arg1, arg2, H));
			double b = (255 * HueToRGB(arg1, arg2, (H - 1.0 / 3.0)));
			Color C((unsigned char)r, (unsigned char)g, (unsigned char)b);
			return C;
		}

	}

	bool operator!=(HSL& first, HSL& second)
	{
		return (first.Hue != second.Hue ||
				first.Saturation != second.Saturation ||
				first.Luminance != second.Luminance);
	}

	/////////////////////////////////////////////////////////////////////
	// Random
	/////////////////////////////////////////////////////////////////////

	namespace
	{
		static std::mt19937 eng;
		static unsigned int engSeed = std::mt19937::default_seed;
	}

	namespace Random
	{
		unsigned int getSeed()
		{
			return engSeed;
		}

		void setSeed(unsigned int seed)
		{
			eng.seed(seed);
			engSeed = seed;
		}

		void setRandomSeed()
		{
			std::random_device rd;
			setSeed(rd());
		}

		int between(int min, int max)
		{
			std::uniform_int_distribution<int> dist(min, max);
			return dist(eng);
		}

		float random(float max)
		{
			std::uniform_real_distribution<float> dist(0.0f, max);
			return dist(eng);
		}

		float betweenf(float min, float max)
		{
			std::uniform_real_distribution<float> dist(min, max);
			return dist(eng);
		}
	}

	/////////////////////////////////////////////////////////////////////
	// ParticleDef
	/////////////////////////////////////////////////////////////////////

	const float ParticleSystem::ParticleDef::degToRad = 0.01745329251994329576f;

	ParticleSystem::ParticleDef::ParticleDef()
	{
		Reset();

		particles = new ParticleOutput[MAX_PARTICLES];
		emitters = new ParticleSystem::EmitterDef[MAX_EMITTERS];
	}

	ParticleSystem::ParticleDef::~ParticleDef()
	{
		delete[] particles;
		particles = nullptr;

		delete[] emitters;
		emitters = nullptr;
	}

	unsigned ParticleSystem::ParticleDef::Reset()
	{
		minLife = 1.f;
		maxLife = 1.0f;

		scale = Vector2(1.0f, 1.0f);

		sizeMin = 32.0f;
		sizeMax = 32.0f;
		sizeInc = 0.0f;
		sizeWiggle = 0.0f;

		rotationRelative = false;
		rotationMin = 0.0f;
		rotationMax = 0.0f;
		rotationInc = 0.0f;
		rotationWiggle = 0.0f;

		dirMin = 0.0f;
		dirMax = 0.0f;
		dirInc = 0.0f;
		dirWiggle = 0.0f;

		speedMin = 0.0f;
		speedMax = 0.0f;
		speedInc = 0.0f;
		speedWiggle = 0.0f;

		Velocity = Vector2(0.0f, 0.0f);

		colorDeltaH = 0.0f;
		colorDeltaS = 0.0f;
		colorDeltaL = 0.0f;
		colorDeltaA = 0.0f;
		colorStart = Color();
		colorEnd = Color();

		particle = (unsigned)-1;

		customData = nullptr;
		flagBits = 0x0000;

		unsigned ParticlesLeftBeforeReset = numParticles;

		queueFront = (unsigned)-1;
		queueRear = (unsigned)-1;
		numParticles = 0;
		numEmitters = 0;

		return(ParticlesLeftBeforeReset);
	}

	int ParticleSystem::ParticleDef::ProcessAll(float deltaTime)
	{
		int Result = 0;

		if (deltaTime == 0.0f)
			return Result;

		unsigned numSpawnedParticles = 0;
		Vector2 spawnLocations[ParticleSystem::MAX_PARTICLES];

		// Particles
		for (unsigned i = 0; i < numParticles; i++)
		{
			particles[i].lifeRemaining -= deltaTime;
			if (particles[i].lifeRemaining <= 0.0f)
			{
				if (particle != (unsigned)-1)
				{
					spawnLocations[numSpawnedParticles++] = particles[i].location;
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
			unsigned framesPassed = emitters[i].Update(deltaTime);
			if (framesPassed > 0)
			{
				unsigned particleCount = emitters[i].particleCount * framesPassed;
				for (unsigned j = 0; j < particleCount; j++)
				{
					spawnLocations[numSpawnedParticles++] = emitters[i].GetSpawnLocation();
				}
			}
		}

		for (unsigned i = 0; i < numSpawnedParticles; i++)
		{
			if (addParticle(spawnLocations[i]))
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

	unsigned ParticleSystem::ParticleDef::GetParticleCount()
	{
		return numParticles;
	}

	ParticleSystem::ParticleOutput* ParticleSystem::ParticleDef::GetParticle(unsigned particleIndex)
	{
		if (particleIndex > numParticles)
			return &particles[0];

		return &particles[particleIndex];
	}

	// Private

	void ParticleSystem::ParticleDef::Process(ParticleOutput& output, float deltaTime)
	{
		//Size
		if (flagBits & EParticleFlags::Flag_Size)
		{
			output.size = updateSize(output.size, deltaTime);
		}

		// Color
		if (flagBits & EParticleFlags::Flag_Color)
			updateColor(output, deltaTime);

		// Velocity
		if (flagBits & EParticleFlags::Flag_Location)
		{
			bool shouldUpdateSpeed = 
				((flagBits & EParticleFlags::Flag_Speed) ? true : false);
			bool shouldUpdateDirection = 
				((flagBits & EParticleFlags::Flag_Direction) ? true : false);
			float currentSpeed = 0.0f;
			float currentDirection = 0.0f;
			ParticleOutput::LocationData locationData;

			if (shouldUpdateSpeed || shouldUpdateDirection)
			{
				locationData = output.locationData;
				currentSpeed = locationData.speed();
				currentDirection = locationData.direction();
			}

			// Speed
			if (shouldUpdateSpeed) 
			{
				currentSpeed = updateSpeed(currentSpeed, deltaTime);
			}

			// Direction
			if (shouldUpdateDirection)
			{
				currentDirection = updateDirection(currentDirection, deltaTime);
			}

			// Velocity
			if (shouldUpdateSpeed || shouldUpdateDirection)
			{
				locationData.speed() = currentSpeed;
				locationData.direction() = currentDirection;
				output.locationData = locationData;

				updateVelocity(currentSpeed, currentDirection);
			}

			// Location
			output.location += Velocity * deltaTime;
		}
		else if (flagBits & EParticleFlags::Flag_ConstVel)
		{
			output.location += output.locationData.Velocity * deltaTime;
		}
		
		// Rotation
		if (flagBits & EParticleFlags::Flag_Rotation)
			updateRotation(output, deltaTime);
	}

	float ParticleSystem::ParticleDef::updateSize(float currentSize, float deltaTime)
	{
		float Result = currentSize + sizeInc*deltaTime;
		return(Result);
	}

	void ParticleSystem::ParticleDef::updateColor(ParticleOutput& output,float deltaTime)
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
		if (flagBits & EParticleFlags::Flag_HSL)
		{
			colorH += colorDeltaH * deltaTime * data.deltaFactor;
			colorS += colorDeltaS * deltaTime * data.deltaFactor;
			colorL += colorDeltaL * deltaTime * data.deltaFactor;

			if (colorH > 360.0f) colorH = 360.0f;
			if (colorH < 0.0f)   colorH = 0.0f;
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

	float ParticleSystem::ParticleDef::updateSpeed(float currentSpeed, float deltaTime)
	{
		float Result = currentSpeed + (speedInc * deltaTime);
		return(Result);
	}

	float ParticleSystem::ParticleDef::updateDirection(float currentDirection, float deltaTime)
	{
		float Result = currentDirection + (dirInc * deltaTime);
		return(Result);
	}

	void ParticleSystem::ParticleDef::updateVelocity(float currentSpeed, float currentDirection)
	{
		float angle = currentDirection * degToRad;
		Vector2 dir(cos(angle), sin(angle));

		float hyp = sqrtf(dir.X*dir.X + dir.Y*dir.Y);
		if (hyp > 0.0f)
		{
			dir.X /= hyp;
			dir.X /= hyp;
		}

		Velocity.X = dir.X*currentSpeed;
		Velocity.Y = dir.Y*currentSpeed;
	}

	void ParticleSystem::ParticleDef::updateRotation(ParticleOutput& output, float deltaTime)
	{
		if (rotationRelative)
			output.rotation = output.locationData.direction();
		else
		{
			output.rotation += rotationInc * deltaTime;
		}
	}

	bool ParticleSystem::ParticleDef::addParticle(Vector2 location)
	{
		if (numParticles >= MAX_PARTICLES)
			return false;

		unsigned slot = numParticles;
		initParticle(particles[slot]);
		particles[slot].location = location;

		numParticles++;

		return true;
	}

	bool ParticleSystem::ParticleDef::removeParticle(unsigned particleIndex)
	{
		if (particleIndex > numParticles)
			return false;

		ParticleOutput lastElement = particles[numParticles];
		particles[numParticles] = particles[particleIndex];
		particles[particleIndex] = lastElement;

		numParticles--;

		return true;
	}

	void ParticleSystem::ParticleDef::initParticle(ParticleOutput& output)
	{
		output.lifeRemaining = Random::betweenf(minLife, maxLife);
		output.size = Random::betweenf(sizeMin, sizeMax);
		output.rotation = Random::betweenf(rotationMin, rotationMax);
		output.scale = scale;
		
		output.colorData.Set(colorStart, colorStartAlpha);
		output.colorData.deltaFactor = (1.0f / output.lifeRemaining);
		output.color = colorStart.TurnToRGB();

		output.customData = customData;

		// Individual static velocity
		if (flagBits & EParticleFlags::Flag_ConstVel)
		{
			output.locationData.Velocity = Velocity;
			return;
		}
		
		output.locationData.direction() = Random::betweenf(dirMin, dirMax);
		output.locationData.speed() = Random::betweenf(speedMin, speedMax);
		
		if (speedMax != 0.0f && speedMin != speedMax) // Has speed?
		{
			// Has no variying velocity
			if ((flagBits & EParticleFlags::Flag_Direction) == false
				&& (flagBits & EParticleFlags::Flag_Speed) == false)
			{
				updateVelocity(output.locationData.speed(), output.locationData.direction());
				output.locationData.Velocity = Velocity;

				flagBits |= EParticleFlags::Flag_ConstVel;
			}
		}
	}

	/////////////////////////////////////////////////////////////////////
	// EmitterDef
	/////////////////////////////////////////////////////////////////////

	const float ParticleSystem::EmitterDef::PI_2 = 6.28318530717958647693f;

	ParticleSystem::EmitterDef::EmitterDef()
	{
		Reset();
	}

	void ParticleSystem::EmitterDef::Reset()
	{
		timer = 0.0f;
		frequency = 1.0f;
		particleCount = 1;
		shape = EmitterShape::POINT;
	}

	unsigned ParticleSystem::EmitterDef::Update(float deltaTime)
	{
		timer += deltaTime;
		if (timer >= frequency)
		{
			timer = 0.0f;
			return 1;
		}

		return 0;
	}

	Vector2 ParticleSystem::EmitterDef::GetSpawnLocation()
	{
		switch (shape)
		{
			case EmitterShape::POINT:
			{
				return location;
			}
			break;
			case EmitterShape::CIRCLE:
			{
				Vector2 Location;

				float angle = Random::betweenf(0.f, PI_2);
				float radius = Random::betweenf(0.0f, dims.X);

				Location.X = location.X + radius * cos(angle);
				Location.Y = location.Y + radius * sin(angle);

				return Location;
			}
			break;
			case EmitterShape::RECTANGLE:
			{
				return location +
					Vector2(dims.X*-1.0f + Random::random(dims.X*2.0f),
							dims.Y*-1.0f + Random::random(dims.Y*2.0f));
			}
			break;
		}

		return Vector2();
	}

	/////////////////////////////////////////////////////////////////////
	// ParticleSystem
	/////////////////////////////////////////////////////////////////////

	ParticleSystem::ParticleSystem()
	{
		Random::setRandomSeed();

		numDefinitions = 0;
		particleDefinitions = new ParticleDef[MAX_DEFINITIONS];

		numParticles = 0;
	}

	ParticleSystem::~ParticleSystem()
	{
		delete[] particleDefinitions;
		particleDefinitions = nullptr;
	}

	void ParticleSystem::Update(float deltaTime)
	{
		for (unsigned i = 0; i < numDefinitions; i++)
		{
			numParticles += particleDefinitions[i].ProcessAll(deltaTime);
		}
	}

	Particle ParticleSystem::CreateParticle()
	{
		Particle handle;
		handle.uniqueID = numDefinitions;
		handle.valid = true;
		numDefinitions++;
		return handle;
	}

	Emitter ParticleSystem::CreateEmitter(Particle spawnedParticle)
	{
		Emitter handle;
		handle.particleID = spawnedParticle.uniqueID;

		unsigned currentEmitterCount = particleDefinitions[handle.particleID].numEmitters;
		handle.valid = (currentEmitterCount < MAX_EMITTERS);
		if (handle.valid)
		{
			handle.uniqueID = currentEmitterCount;
			particleDefinitions[handle.particleID].numEmitters++;
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

		particleDefinitions[emitter.particleID].emitters[emitter.uniqueID].Reset();
	}

	void ParticleSystem::SpawnParticle(Particle particle, Vector2 location, unsigned spawnCount)
	{
		if (particle.valid == false)
			return;

		for (unsigned i = 0; i < spawnCount; i++)
		{
			if (particleDefinitions[particle.uniqueID].addParticle(location))
				numParticles++;
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

	void ParticleSystem::EmitterSetLocation(Emitter emitter, Vector2 location)
	{
		if (emitter.valid == false)
			return;

		EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];
		def.location = location;
	}

	void ParticleSystem::EmitterSetPoint(Emitter emitter, Vector2 location)
	{
		if (emitter.valid == false)
			return;

		EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];
		def.shape = EmitterDef::EmitterShape::POINT;
		def.location = location;
	}

	void ParticleSystem::EmitterSetCircle(Emitter emitter, Vector2 location, float radius)
	{
		if (emitter.valid == false)
			return;

		EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];
		def.shape = EmitterDef::EmitterShape::CIRCLE;
		def.location = location;
		def.dims = Vector2(radius, radius);
	}

	void ParticleSystem::EmitterSetRectangle(Emitter emitter, Vector2 location, Vector2 dimension)
	{
		if (emitter.valid == false)
			return;

		EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];

		def.shape = EmitterDef::EmitterShape::RECTANGLE;
		def.location = location;
		def.dims = dimension;
	}

	void ParticleSystem::EmitterSetFrequency(Emitter emitter, float frequency, unsigned spawnCount)
	{
		if (emitter.valid == false)
			return;

		EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];
		def.frequency = frequency;
		def.particleCount = spawnCount;
	}

	void ParticleSystem::ParticleSetLifetime(Particle& particle, float minLife, float maxLife)
	{
		if (particle.valid == false)
			return;

		if (minLife == 0.0f && maxLife == 0.0f)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.minLife = std::min(minLife, maxLife);
		def.maxLife = std::max(minLife, maxLife);
	}

	void ParticleSystem::ParticleSetSize(Particle& particle, float sizeMin, float sizeMax, float sizeInc, float sizeWiggle)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.sizeMin = std::min(sizeMin, sizeMax);
		def.sizeMax = std::max(sizeMin, sizeMax);
		def.sizeInc = sizeInc;
		def.sizeWiggle = sizeWiggle;

		if (sizeInc != 0.0f || sizeWiggle != 0.0f)
			def.flagBits |= EParticleFlags::Flag_Size;
	}

	void ParticleSystem::ParticleSetRotation(Particle& particle, float rotMin, float rotMax, float rotInc /*= 0.0f*/, float rotWiggle /*= 0.0f*/, bool rotRelative /*= false*/)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.rotationMin = std::min(rotMin, rotMax);
		def.rotationMax = std::max(rotMin, rotMax);
		def.rotationInc = rotInc;
		def.rotationWiggle = rotWiggle;
		def.rotationRelative = rotRelative;

		if (rotInc != 0.0f || rotWiggle != 0.0f)
			def.flagBits |= EParticleFlags::Flag_Rotation;
	}

	void ParticleSystem::ParticleSetScale(Particle& particle, float scaleX, float scaleY)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.scale = Vector2(scaleX, scaleY);
	}

	void ParticleSystem::ParticleSetColor(Particle& particle, Color color)
	{
		if (particle.valid == false)
			return;

		ParticleSetColor(particle, color, color);
	}

	void ParticleSystem::ParticleSetColor(Particle& particle, Color colorStart, Color colorEnd)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.colorStart = HSL(colorStart);
		def.colorEnd = HSL(colorEnd);
		def.colorStartAlpha = colorStart.A;
		def.colorEndAlpha = colorEnd.A;

		if (def.colorStart != def.colorEnd)
		{
			if (def.colorStart.Hue != def.colorEnd.Hue)
				def.colorDeltaH = float(def.colorStart.Hue - def.colorEnd.Hue) * -1.0f;
			if (def.colorStart.Saturation != def.colorEnd.Saturation)
				def.colorDeltaS = float(def.colorStart.Saturation - def.colorEnd.Saturation) * -1.0f;
			if (def.colorStart.Luminance != def.colorEnd.Luminance)
				def.colorDeltaL = float(def.colorStart.Luminance - def.colorEnd.Luminance) * -1.0f;
		
			def.flagBits |= EParticleFlags::Flag_HSL;
			def.flagBits |= EParticleFlags::Flag_Color;
		}
		
		if (colorStart.A != colorEnd.A)
		{
			def.colorDeltaA = float(def.colorStartAlpha - def.colorEndAlpha) * -1.0f;

			def.flagBits |= EParticleFlags::Flag_Color;
		}
		else
		{
			def.colorDeltaA = 0.f;
		}
	}

	void ParticleSystem::ParticleSetDirection(Particle& particle, float dirMin, float dirMax, float dirInc, float dirWiggle)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.dirMin = std::min(dirMin, dirMax);
		def.dirMax = std::max(dirMin, dirMax);
		def.dirInc = dirInc;
		def.dirWiggle = dirWiggle;

		if (dirInc != 0.0f || dirWiggle != 0.0f)
		{
			def.flagBits |= EParticleFlags::Flag_Direction;
			def.flagBits |= EParticleFlags::Flag_Location;
		}
		else if (def.dirMax != 0.0f && def.dirMin == def.dirMax)
		{
			def.updateVelocity(def.speedMax, def.dirMax);
			def.flagBits |= EParticleFlags::Flag_Location;
		}
	}

	void ParticleSystem::ParticleSetSpeed(Particle& particle, float speedMin, float speedMax, float speedInc, float speedWiggle)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.speedMin = std::min(speedMin, speedMax);
		def.speedMax = std::max(speedMin, speedMax);
		def.speedInc = speedInc;
		def.speedWiggle = speedWiggle;

		if (speedInc != 0.0f || speedWiggle != 0.0f)
		{
			def.flagBits |= EParticleFlags::Flag_Speed;
			def.flagBits |= EParticleFlags::Flag_Location;
		}
		else if (def.speedMax != 0.0f && def.speedMin == def.speedMax)
		{
			def.updateVelocity(def.speedMax, def.dirMax);
			def.flagBits |= EParticleFlags::Flag_Location;
		}
	}

	void ParticleSystem::ParticleSetVelocity(Particle& particle, Vector2 velocity)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.Velocity = velocity;

		if (velocity != Vector2(0.0f, 0.0f))
		{
			def.flagBits |= EParticleFlags::Flag_Location;
			def.flagBits &= ~EParticleFlags::Flag_Speed;
			def.flagBits &= ~EParticleFlags::Flag_Direction;
		}
	}

	void ParticleSystem::ParticleSetSpawnedParticle(Particle& particle, Particle spawnedParticle)
	{
		if (particle.valid == false)
			return;

		particleDefinitions[particle.uniqueID].particle = spawnedParticle.uniqueID;
	}

	void ParticleSystem::ParticleSetCustomData(Particle& particle, void* data)
	{
		if (particle.valid == false)
			return;

		particleDefinitions[particle.uniqueID].customData = data;
	}

	unsigned ParticleSystem::GetDefinitionCount()
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

	ParticleSystem::ParticleOutput* ParticleSystem::GetParticle(unsigned particleIndex)
	{
		if (particleIndex > particleDefinitions[0].numParticles)
			return &particleDefinitions[0].particles[particleDefinitions[0].numParticles];

		return &particleDefinitions[0].particles[particleIndex];
	}

	// Private

	ParticleSystem::ParticleDef* ParticleSystem::getDefinitionFromIndex(unsigned& index)
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

	/////////////////////////////////////////////////////////////////////
	// ParticleIterator
	/////////////////////////////////////////////////////////////////////

	ParticleIterator::ParticleIterator(class ParticleSystem& particleSystem)
	{
		partSystem = &particleSystem;

		numDefinitions = partSystem->numDefinitions;
		defIndex = 0;
		numParticlesInDef = 0;
		particleIndex = 0;

		target = nullptr;
		currentDef = nullptr;

		reachedEnd = partSystem->GetSpawnedParticleCount() == 0 ? true : false;

		if (reachedEnd == false)
		{
			currentDef = partSystem->getDefinitionFromIndex(defIndex);
			if (currentDef != nullptr)
			{
				numParticlesInDef = currentDef->GetParticleCount();
				target = currentDef->GetParticle(particleIndex);
			}
			else
			{
				reachedEnd = true;
			}
		}
	}

	void ParticleIterator::operator++()
	{
		if (particleIndex < numParticlesInDef)
		{
			particleIndex++;
			target = currentDef->GetParticle(particleIndex);

			return;
		}
		else
		{
			defIndex++;
			currentDef = partSystem->getDefinitionFromIndex(defIndex);
			if (currentDef == nullptr)
			{
				reachedEnd = true;

				return;
			}
			else
			{
				particleIndex = 0;
				numParticlesInDef = currentDef->GetParticleCount();
				target = currentDef->GetParticle(particleIndex);

				return;
			}
		}
	}

	void ParticleIterator::operator++(int)
	{
		++(*this);
	}

	const Output& ParticleIterator::operator*() const
	{
		return *target;
	}

	const Output& ParticleIterator::operator->() const
	{
		return *target;
	}

	ParticleIterator::operator bool() const
	{
		return !reachedEnd;
	}

	/////////////////////////////////////////////////////////////////////
	// EmitterIterator
	/////////////////////////////////////////////////////////////////////

	EmitterIterator::EmitterIterator(class ParticleSystem& particleSystem)
	{
		partSystem = &particleSystem;

		numDefinitions = partSystem->numDefinitions;
		defIndex = 0;
		numEmittersInDef = 0;
		emitterIndex = 0;

		target = nullptr;
		currentDef = nullptr;

		reachedEnd = partSystem->GetSpawnedParticleCount() == 0 ? true : false;

		if (reachedEnd == false)
		{
			currentDef = partSystem->getDefinitionFromIndex(defIndex);
			if (currentDef != nullptr)
			{
				numEmittersInDef = currentDef->GetParticleCount();
				target = currentDef->GetParticle(emitterIndex);
			}
			else
			{
				reachedEnd = true;
			}
		}
	}

	void EmitterIterator::operator++()
	{
		if (emitterIndex < numEmittersInDef)
		{
			emitterIndex++;
			target = currentDef->GetParticle(emitterIndex);

			return;
		}
		else
		{
			defIndex++;
			currentDef = partSystem->getDefinitionFromIndex(defIndex);
			if (currentDef == nullptr)
			{
				reachedEnd = true;

				return;
			}
			else
			{
				emitterIndex = 0;
				numEmittersInDef = currentDef->GetParticleCount();
				target = currentDef->GetParticle(emitterIndex);

				return;
			}
		}
	}

	void EmitterIterator::operator++(int)
	{
		++(*this);
	}

	const Output& EmitterIterator::operator*() const
	{
		return *target;
	}

	const Output& EmitterIterator::operator->() const
	{
		return *target;
	}

	EmitterIterator::operator bool() const
	{
		return !reachedEnd;
	}
};