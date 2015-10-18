// ParticleSystem.cpp

#include "ParticleSystem.h"

#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>

#include <stdlib.h>

namespace PS
{
	/////////////////////////////////////////////////////////////////////
	// Vector2
	/////////////////////////////////////////////////////////////////////

	Vector2::Vector2()
		: X(0.0f)
		, Y(0.0f)
	{
	}

	Vector2::Vector2(float x, float y)
		: X(x)
		, Y(y)
	{
	}

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
	{
	}

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
	{
	}

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
	}

	void ParticleSystem::ParticleDef::Reset()
	{
		minLife = 1.f;
		maxLife = 1.0f;

		scale = Vector2(1.0f, 1.0f);

		sizeMin = 1.0f;
		sizeMax = 1.0f;
		sizeInc = 0.0f;
		sizeWiggle = 0.0f;

		rotationRelative = false;
		rotationMin = 0.0f;
		rotationMax = 0.0f;
		rotationInc = 0.0f;
		rotationWiggle = 0.0f;

		directionMin = 0.0f;
		directionMax = 0.0f;
		directionInc = 0.0f;
		directionWiggle = 0.0f;

		speedMin = 0.0f;
		speedMax = 0.0f;
		speedInc = 0.0f;
		speedWiggle = 0.0f;

		Velocity = Vector2(0.0f, 0.0f);

		colorDeltaH = 0.0f;
		colorDeltaS = 0.0f;
		colordeltaL = 0.0f;
		colorDeltaA = 0.0f;
		colorStart = Color();
		colorEnd = Color();

		particle = (unsigned)-1;

		customData = nullptr;

		m_flagBits = 0x0000;
	}

	void ParticleSystem::ParticleDef::Process(ParticleOutput& output, float deltaTime)
	{
		//Size
		if (m_flagBits & EParticleFlags::Flag_Size)
		{
			output.size = updateSize(output.size, deltaTime);
		}

		// Color
		if (m_flagBits & EParticleFlags::Flag_Color)
			updateColor(output, deltaTime);

		// Location
		if (m_flagBits & EParticleFlags::Flag_Location)
		{
			bool shouldUpdateSpeed = 
				((m_flagBits & EParticleFlags::Flag_Speed) ? true : false);
			bool shouldUpdateDirection = 
				((m_flagBits & EParticleFlags::Flag_Direction) ? true : false);
			float currentSpeed = 0.0f;
			float currentDirection = 0.0f;
			ParticleOutput::LocationData locationData;

			if (shouldUpdateSpeed || shouldUpdateDirection)
			{
				locationData = output.m_locationData;
				currentSpeed = locationData.speed;
				currentDirection = locationData.direction;
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
				locationData.speed = currentSpeed;
				locationData.direction = currentDirection;
				output.m_locationData = locationData;

				updateVelocity(currentSpeed, currentDirection, deltaTime);
			}

			// Location
			output.location += Velocity * deltaTime;
		}
		
		// Rotation
		if (m_flagBits & EParticleFlags::Flag_Rotation)
			updateRotation(output, deltaTime);
	}

	// Private

	float ParticleSystem::ParticleDef::updateSize(float currentSize, float deltaTime)
	{
		float Result = currentSize + sizeInc*deltaTime;
		return(Result);
	}

	void ParticleSystem::ParticleDef::updateColor(ParticleOutput& output,float deltaTime)
	{
		ParticleOutput::ColorData data = output.m_colorData;
		float colorH = data.H;
		float colorS = data.S;
		float colorL = data.L;
		float colorA = data.A;

		colorH += colorDeltaH * deltaTime;
		colorS += colorDeltaS * deltaTime;
		colorL += colordeltaL * deltaTime;
		colorA += colorDeltaA * deltaTime;

		if (colorH > 360.0f) colorH = 360.0f;
		if (colorH < 0.0f)   colorH = 0.0f;
		if (colorS > 100.0f) colorS = 100.0f;
		if (colorS< 0.0f)    colorS = 0.0f;
		if (colorL > 100.0f) colorL = 100.0f;
		if (colorL < 0.0f)   colorL = 0.0f;
		if (colorA > 360.0f) colorA = 360.0f;
		if (colorA < 0.0f)   colorA = 0.0f;

		data.H = colorH;
		data.S = colorS;
		data.L = colorL;
		data.A = colorA;

		Color Result = HSL((unsigned)colorH, (unsigned)colorS, (unsigned)colorL).TurnToRGB();
		Result.A = (unsigned char)colorA;

		output.color = Result;
	}

	float ParticleSystem::ParticleDef::updateSpeed(float currentSpeed, float deltaTime)
	{
		float Result = currentSpeed + (speedInc * deltaTime);
		return(Result);
	}

	float ParticleSystem::ParticleDef::updateDirection(float currentDirection, float deltaTime)
	{
		float Result = currentDirection + (directionInc * deltaTime);
		return(Result);
	}

	void ParticleSystem::ParticleDef::updateVelocity(float currentSpeed, float currentDirection, float deltaTime)
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
			output.rotation = output.m_locationData.direction;
		else
		{
			output.rotation += rotationInc * deltaTime;
		}
	}

	/////////////////////////////////////////////////////////////////////
	// EmitterDef
	/////////////////////////////////////////////////////////////////////

	const float ParticleSystem::Emitterdef::PI_2 = 6.28318530717958647693f;

	ParticleSystem::Emitterdef::Emitterdef()
	{
		Reset();
	}

	void ParticleSystem::Emitterdef::Reset()
	{
		particle = (unsigned)-1;
		timer = 0.0f;
		frequency = 1.0f;
		shape = EmitterShape::POINT;
	}

	bool ParticleSystem::Emitterdef::Update(float deltaTime)
	{
		timer += deltaTime;
		if (timer >= frequency)
		{
			timer = 0.0f;
			return true;
		}

		return false;
	}

	Vector2 ParticleSystem::Emitterdef::GetSpawnLocation()
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
					Vector2(dims.X*-1.0f + Random::random(dims.X),
							dims.Y*-1.0f + Random::random(dims.Y));
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
		particleDefinitions = new ParticleDef[MAX_PARTICLE_DEFS];
		for (unsigned i = 0; i < MAX_PARTICLE_DEFS; i++)
		{
			particleDefinitions[i] = ParticleDef();
		}
		//numFreeDefinitionSlots = 0;

		particleDefCacheIndex = (unsigned)-1;

		numEmitters = 0;
		emitters = new Emitterdef[MAX_EMITTERS];
		//numFreeEmitterSlots = 0;

		numParticles = 0;
		particles = new ParticleOutput[MAX_PARTICLES];
		//numFreeParticleSlots = 0;
	}

	ParticleSystem::~ParticleSystem()
	{
		delete[] particleDefinitions;
		particleDefinitions = nullptr;
		delete[] emitters;
		emitters = nullptr;
		delete[] particles;
		particles = nullptr;
	}

	void ParticleSystem::Update(float deltaTime)
	{
		unsigned numCreatedParticles = 0;
		unsigned createdParticles[MAX_PARTICLES];
		Vector2 createdParticleLocations[MAX_PARTICLES];
		unsigned numDestroyedParticles = 0;
		unsigned destroyedParticles[MAX_PARTICLES];

		for (unsigned i = 0; i < numParticles; i++)
		{
			particles[i].m_life -= deltaTime;
			if (particles[i].m_life <= 0.0f)
			{
				destroyedParticles[numDestroyedParticles] = i;
				numDestroyedParticles++;

				/*if (particles[i].data.particle != -1)
				{
					createdParticles[numCreatedParticles] = particles[i].data.particle;
					createdParticleLocations[numCreatedParticles] = particles[i].location;
					numCreatedParticles++;
				}*/
				continue;
			}

			particleDefinitions[particles[i].m_def].Process(particles[i], deltaTime);
		}

		for (unsigned i = 0; i < numEmitters; i++)
		{
			if (emitters[i].Update(deltaTime))
			{
				createdParticles[numCreatedParticles] = emitters[i].particle;
				createdParticleLocations[numCreatedParticles] = emitters[i].GetSpawnLocation();
				numCreatedParticles++;
			}
		}

		for (unsigned i = 0; i < numDestroyedParticles; i++)
		{
			removeParticle(destroyedParticles[i]);
		}

		for (unsigned i = 0; i < numCreatedParticles; i++)
		{
			addParticle(createdParticles[i], createdParticleLocations[i]);
		}
	}

	Particle ParticleSystem::CreateParticle()
	{
		Particle handle;
		handle.uniqueID = //getFreeDefenitionSlot();
		numDefinitions++;
		return handle;
	}

	Emitter ParticleSystem::CreateEmitter()
	{
		Emitter handle;
		handle.uniqueID = numEmitters;// getFreeEmitterSlot();
		numEmitters++;
		return handle;
	}

	void ParticleSystem::DestroyParticle(Particle particle)
	{
		particleDefinitions[particle.uniqueID].Reset();

		if (particleDefCacheIndex == particle.uniqueID)
		{
			particleDefCacheIndex = (unsigned)-1;
		}

		//addFreeDefinitionSlot(particle.uniqueID);
		numDefinitions--;
	}

	void ParticleSystem::DestroyEmitter(Emitter emitter)
	{
		emitters[emitter.uniqueID].Reset();

		//addFreeEmitterSlot(emitter.uniqueID);
		numEmitters--;
	}

	void ParticleSystem::EmitterSetParticle(Emitter emitter, Particle particle)
	{
		emitters[emitter.uniqueID].particle = particle.uniqueID;
	}

	void ParticleSystem::EmitterSetLocation(Emitter emitter, Vector2 location)
	{
		emitters[emitter.uniqueID].location = location;
	}

	void ParticleSystem::EmitterSetPoint(Emitter emitter, Vector2 location)
	{
		emitters[emitter.uniqueID].shape = Emitterdef::EmitterShape::POINT;
		emitters[emitter.uniqueID].location = location;
	}

	void ParticleSystem::EmitterSetCircle(Emitter emitter, Vector2 location, float radius)
	{
		emitters[emitter.uniqueID].shape = Emitterdef::EmitterShape::CIRCLE;
		emitters[emitter.uniqueID].location = location;
		emitters[emitter.uniqueID].dims = Vector2(radius, radius);
	}

	void ParticleSystem::EmitterSetRectangle(Emitter emitter, Vector2 location, Vector2 dimension)
	{
		emitters[emitter.uniqueID].shape = Emitterdef::EmitterShape::RECTANGLE;
		emitters[emitter.uniqueID].location = location;
		emitters[emitter.uniqueID].dims = dimension;
	}

	void ParticleSystem::EmitterSetFrequency(Emitter emitter, float frequency)
	{
		emitters[emitter.uniqueID].frequency = frequency;
	}

	void ParticleSystem::ParticleSetLifetime(Particle particle, float minLife, float maxLife)
	{
		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.minLife = std::min(minLife, maxLife);
		def.maxLife = std::max(minLife, maxLife);
	}

	void ParticleSystem::ParticleSetSize(Particle particle, float sizeMin, float sizeMax, float sizeInc, float sizeWiggle)
	{
		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.sizeMin = std::min(sizeMin, sizeMax);
		def.sizeMax = std::max(sizeMin, sizeMax);
		def.sizeInc = sizeInc;
		def.sizeWiggle = sizeWiggle;

		if (sizeMin != sizeMax || sizeInc != 0.0f || sizeWiggle != 0.0f)
			def.m_flagBits |= EParticleFlags::Flag_Size;
	}

	void ParticleSystem::ParticleSetRotation(Particle particle, float rotMin, float rotMax, float rotInc /*= 0.0f*/, float rotWiggle /*= 0.0f*/, bool rotRelative /*= false*/)
	{
		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.rotationMin = std::min(rotMin, rotMax);
		def.rotationMax = std::max(rotMin, rotMax);
		def.rotationInc = rotInc;
		def.rotationWiggle = rotWiggle;
		def.rotationRelative = rotRelative;

		if (rotMin != rotMax || rotInc != 0.0f || rotWiggle != 0.0f)
			def.m_flagBits |= EParticleFlags::Flag_Rotation;
	}

	void ParticleSystem::ParticleSetScale(Particle particle, float scaleX, float scaleY)
	{
		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.scale = Vector2(scaleX, scaleY);
	}

	void ParticleSystem::ParticleSetColor(Particle particle, Color color)
	{
		ParticleSetColor(particle, color, color);
	}

	void ParticleSystem::ParticleSetColor(Particle particle, Color colorStart, Color colorEnd)
	{
		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.colorStart = HSL(colorStart);
		def.colorEnd = HSL(colorEnd);
		def.colorStartAlpha = colorStart.A;
		def.colorEndAlpha = colorEnd.A;

		if (colorStart != colorEnd || colorStart.A != colorEnd.A)
			def.m_flagBits |= EParticleFlags::Flag_Color;
	}

	void ParticleSystem::ParticleSetVelocity(Particle particle, Vector2 velocity)
	{
		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.Velocity = velocity;

		if (velocity != Vector2(0.0f, 0.0f))
			def.m_flagBits |= EParticleFlags::Flag_Location;
	}

	void ParticleSystem::ParticleSetCustomData(Particle particle, void* data)
	{
		particleDefinitions[particle.uniqueID].customData = data;
	}

	unsigned ParticleSystem::GetParticleCount()
	{
		return numParticles;
	}

	ParticleSystem::ParticleOutput* ParticleSystem::GetParticle(unsigned particleIndex)
	{
		if (particleIndex > numParticles)
			return nullptr;

		return &particles[particleIndex];
	}

	// Private

	void ParticleSystem::addParticle(unsigned defIndex, Vector2 location)
	{
		unsigned slot = numParticles;// getFreeParticleSlot();
		initParticle(particles[slot], defIndex);
		particles[slot].location = location;
		
		numParticles++;
	}

	void ParticleSystem::removeParticle(unsigned particleIndex)
	{
		//addFreeParticleSlot(particleIndex);

		ParticleOutput temp = particles[numParticles];
		particles[numParticles] = particles[particleIndex];
		particles[particleIndex] = temp;

		numParticles--;
	}

	void ParticleSystem::initParticle(ParticleOutput& output, unsigned defIndex)
	{
		ParticleDef def;
		if (defIndex == particleDefCacheIndex)
		{
			def = particleDefCache;
		}
		else
		{
			def = particleDefinitions[defIndex];

			particleDefCacheIndex = defIndex;
			particleDefCache = def;
		}

		output.m_def = defIndex;
		output.m_life = Random::betweenf(def.minLife, def.maxLife);
		output.scale = def.scale;
		output.rotation = Random::betweenf(def.rotationMin, def.rotationMax);
		output.size = Random::betweenf(def.sizeMin, def.sizeMax);
		output.m_colorData.Set(def.colorStart, def.colorStartAlpha);
		output.color = def.colorStart.TurnToRGB();
		output.customData = def.customData;
	}

#if 0
	void ParticleSystem::addFreeDefinitionSlot(unsigned slot)
	{
		numFreeDefinitionSlots++;
		freeDefinitionSlots[numFreeDefinitionSlots] = slot;
	}

	void ParticleSystem::addFreeParticleSlot(unsigned slot)
	{
		numFreeDefinitionSlots++;
		freeParticleSlots[numFreeParticleSlots] = slot;
	}

	void ParticleSystem::addFreeEmitterSlot(unsigned slot)
	{
		numFreeDefinitionSlots++;
		freeEmitterSlots[numFreeEmitterSlots] = slot;
	}

	unsigned ParticleSystem::getFreeDefenitionSlot()
	{
		if (numFreeDefinitionSlots == 0)
		{
			if (numDefinitions == MAX_PARTICLE_DEFS)
				return 0;
			else
				return numDefinitions;
		}

		unsigned slot = freeDefinitionSlots[numFreeDefinitionSlots];
		numFreeDefinitionSlots--;
		return slot;
	}

	unsigned ParticleSystem::getFreeEmitterSlot()
	{
		if (numFreeEmitterSlots == 0)
		{
			if (numEmitters == MAX_EMITTERS)
				return 0;
			else
				return numEmitters;
		}

		unsigned slot = freeEmitterSlots[numFreeEmitterSlots];
		numFreeEmitterSlots--;
		return slot;
	}

	unsigned ParticleSystem::getFreeParticleSlot()
	{
		if (numFreeParticleSlots == 0)
		{
			if (numParticles == MAX_PARTICLES)
				return 0;
			else
				return numParticles;
		}

		unsigned slot = freeParticleSlots[numFreeParticleSlots];
		numFreeParticleSlots--;
		return slot;
	}
#endif
};