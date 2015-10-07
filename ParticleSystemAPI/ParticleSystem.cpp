// ParticleSystem.cpp

#include "ParticleSystem.h"

#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>

namespace PS
{
	/////////////////////////////////////////////////////////////////////
	// Vector2
	/////////////////////////////////////////////////////////////////////

	Vector2::Vector2()
		: X(0)
		, Y(0)
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

	HSL TurnToHSL(const Color& C)
	{
		double R, G, B;
		R = C.R / 255.0;
		G = C.G / 255.0;
		B = C.B / 255.0;
		///Casos no triviales.
		double max, min, l, s = 0.f;

		///Maximos
		max = std::max(std::max(R, G), B);

		///Minimos
		min = std::min(std::min(R, G), B);


		HSL A;
		l = ((max + min) / 2.0);

		if (max - min <= HSL::EPSILON)
		{
			A.Hue = 0;
			A.Saturation = 0;
		}
		else
		{
			double diff = max - min;

			if (A.Luminance < 0.5)
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


			if (max - R <= HSL::EPSILON) { A.Hue = diffB - diffG; }
			else if (max - G <= HSL::EPSILON) { A.Hue = (1 * 360) / 3.0 + (diffR - diffB); }
			else if (max - B <= HSL::EPSILON) { A.Hue = (2 * 360) / 3.0 + (diffG - diffR); }

			if (A.Hue <= 0 || A.Hue >= 360) { fmod(A.Hue, 360); }

			s *= 100;
		}

		l *= 100;
		A.Saturation = s;
		A.Luminance = l;
		return A;
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

		float betweenf(float min, float max)
		{
			std::uniform_real_distribution<float> dist(min, max);
			return dist(eng);
		}
	}

	/////////////////////////////////////////////////////////////////////
	// ParticleDef
	/////////////////////////////////////////////////////////////////////

	ParticleSystem::ParticleDef::ParticleDef()
	{
		minLife = 1.f;
		maxLife = 1.0f;

		scale = Vector2(1.0f, 1.0f);

		sizeConst = true;
		size = 32.0f;
		sizeMin = size;
		sizeMax = size;
		sizeInc = 0.0f;
		sizeWiggle = 0.0f;

		rotationRelative = false;
		rotationConst = true;
		rotation = 0.0f;
		rotationMin = rotation;
		rotationMax = rotation;
		rotationInc = 0.0f;
		rotationWiggle = 0.0f;

		directionConst = true;
		direction = 0.0f;
		directionMin = direction;
		directionMax = direction;
		directionInc = 0.0f;
		directionWiggle = 0.0f;

		speedConst = true;
		speed = 0.0f;
		speedMin = speed;
		speedMax = speed;
		speedInc = 0.0f;
		speedWiggle = 0.0f;

		Velocity = Vector2(0.0f, 0.0f);

		colorConst = true;
		colorH = 360.0f;
		colorS = 100.0f;
		colorL = 100.0f;
		colorA = 1.0f;
		colorDeltaH = 0.0f;
		colorDeltaS = 0.0f;
		colordeltaL = 0.0f;
		colorDeltaA = 0.0f;
		colorStart = Color();
		colorEnd = Color();

		particle = (unsigned)-1;

		customData = nullptr;
	}

	// Private

	void ParticleSystem::ParticleDef::updateSize()
	{
		sizeConst = 
			sizeMin == sizeMax 
			&& sizeWiggle == 0.0f 
			&& sizeInc == 0.0f;
	}

	void ParticleSystem::ParticleDef::updateRotation()
	{
		rotationConst =
			!rotationRelative
			&& rotationMin == rotationMax
			&& rotationInc == 0.0f
			&& rotationWiggle == 0.f;
	}

	void ParticleSystem::ParticleDef::updateVelocity()
	{

	}

	void ParticleSystem::ParticleDef::updateColor()
	{

	}

	/////////////////////////////////////////////////////////////////////
	// EmitterDef
	/////////////////////////////////////////////////////////////////////

	ParticleSystem::Emitterdef::Emitterdef()
	{
		particle = (unsigned)-1;
		timer = 0.0f;
		frequency = 1.0f;
	}

	ParticleSystem::Emitterdef::Emitterdef(EmitterShape newShape)
	{
		shape = newShape;
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
		Vector2 Location;
		switch (shape)
		{
			case EmitterShape::POINT:
				Location = location;
			break;
			case EmitterShape::CIRCLE:

			break;
			case EmitterShape::RECTANGLE:
				
			break;
		}

		return Location;
	}

	/////////////////////////////////////////////////////////////////////
	// ParticleData
	/////////////////////////////////////////////////////////////////////

	ParticleSystem::ParticleData::ParticleData()
	{
		lifeLeft = 1.0f;
		
		size = 8.f;
		rotation = 0.f;
		direction = 0.f;
		speed = 0.f;

		colorH = 360.f;
		colorS = 100.f;
		colorL = 100.f;
		colorA = 1.f;
	}

	void ParticleSystem::ParticleData::Init(ParticleDef def, Vector2 newLocation)
	{
		data = def;
		location = newLocation;
		lifeLeft = data.maxLife;

		size = Random::betweenf(data.sizeMin, data.sizeMax);
	}

	bool ParticleSystem::ParticleData::Update(float deltaTime)
	{
		if (deltaTime == 0.0f)
			return false;

		lifeLeft -= deltaTime;
		if (lifeLeft <= 0)
		{
			return true;
		}

		if (!data.colorConst)
		{
			colorH += data.colorDeltaH * deltaTime;
			colorS += data.colorDeltaS * deltaTime;
			colorL += data.colordeltaL * deltaTime;
			colorA += data.colorDeltaA * deltaTime;

			if (colorH > 360.0f) colorH = 360.0f;
			if (colorH < 0.0f)   colorH = 0.0f;
			if (colorS > 100.0f) colorS = 100.0f;
			if (colorS< 0.0f)    colorS = 0.0f;
			if (colorL > 100.0f) colorL = 100.0f;
			if (colorL < 0.0f)   colorL = 0.0f;
			if (colorA > 360.0f) colorA = 360.0f;
			if (colorA < 0.0f)   colorA = 0.0f;
		}

		// TRANSFORMATIONS
		if (!data.sizeConst)
		{
			size += Random::betweenf(data.sizeWiggle * -1.0f, data.sizeWiggle);
			size += data.sizeInc * deltaTime;
		}

		if (!data.rotationConst && !data.rotationRelative)
		{
			rotation += Random::betweenf(data.rotationWiggle * -1.0f, data.rotationWiggle);
			rotation += data.rotationInc * deltaTime;
		}

		if (!data.directionConst)
		{
			direction += Random::betweenf(data.directionWiggle * -1.0f, data.directionWiggle);
			direction += data.directionInc * deltaTime;
		}

		if (!data.speedConst)
		{
			speed += Random::betweenf(data.speedWiggle * -1.0f, data.speedWiggle);
			speed += data.speedInc * deltaTime;
		}

		return false;
	}

	Color ParticleSystem::ParticleData::GetColor()
	{
		return Color(255, 0, 0, 255);
		Color ReturnColor = HSL((unsigned)colorH, (unsigned)colorS, (unsigned)colorL).TurnToRGB();
		ReturnColor.A = (unsigned char)colorA;
		return ReturnColor;
	}

	/////////////////////////////////////////////////////////////////////
	// ParticleSystem
	/////////////////////////////////////////////////////////////////////

	ParticleSystem::ParticleSystem()
	{
		Random::setRandomSeed();

		numDefenitions = 0;
		numFreeDefenitionSlots = 0;

		numParticles = 0;
		numFreeParticleSlots = 0;

		numEmitters = 0;
		numFreeEmitterSlots = 0;
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
			float lifeLeft = particles[i].lifeLeft;
			lifeLeft -= deltaTime;
			if (lifeLeft <= 0.0f)
			{
				numDestroyedParticles++;
				destroyedParticles[numDestroyedParticles] = i;

				if (particles[i].data.particle != -1)
				{
					createdParticles[numCreatedParticles] = particles[i].data.particle;
					createdParticleLocations[numCreatedParticles] = particles[i].location;
					numCreatedParticles++;
				}
			}
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
		handle.uniqueID = getFreeDefenitionSlot();
		numDefenitions++;
		return handle;
	}

	Emitter ParticleSystem::CreateEmitter()
	{
		Emitter handle;
		handle.uniqueID = getFreeEmitterSlot();
		numEmitters++;
		return handle;
	}

	void ParticleSystem::DestroyParticle(Particle particle)
	{
		addFreeDefinitionSlot(particle.uniqueID);
		numDefenitions--;
	}

	void ParticleSystem::DestroyEmitter(Emitter emitter)
	{
		addFreeEmitterSlot(emitter.uniqueID);
		numEmitters--;
	}

	void ParticleSystem::ParticleSetLifetime(Particle particle, float minLife, float maxLife)
	{
		ParticleDef& def = particleDefenitions[particle.uniqueID];
		def.minLife = minLife;
		def.maxLife = maxLife;
	}

	void ParticleSystem::ParticleSetSize(Particle particle, float sizeMin, float sizeMax, float sizeInc, float sizeWiggle)
	{
		ParticleDef& def = particleDefenitions[particle.uniqueID];
		def.sizeMin = sizeMin;
		def.sizeMax = sizeMax;
		def.sizeInc = sizeInc;
		def.sizeWiggle = sizeWiggle;
	}

	void ParticleSystem::ParticleSetScale(Particle particle, float scaleX, float scaleY)
	{
		ParticleDef& def = particleDefenitions[particle.uniqueID];
		def.scale = Vector2(scaleX, scaleY);
	}

	void ParticleSystem::ParticleSetColor(Particle particle, Color color)
	{
		ParticleSetColor(particle, color, color);
	}

	void ParticleSystem::ParticleSetColor(Particle particle, Color colorStart, Color colorEnd)
	{
		ParticleDef& def = particleDefenitions[particle.uniqueID];
		def.colorStart = colorStart;
		def.colorEnd = colorEnd;
	}

	void ParticleSystem::ParticleSetVelocity(Particle particle, Vector2 velocity)
	{
		particleDefenitions[particle.uniqueID].Velocity = velocity;
	}

	void ParticleSystem::ParticleSetCustomData(Particle particle, void* data)
	{
		particleDefenitions[particle.uniqueID].customData = data;
	}

	void ParticleSystem::EmitterSetParticle(Emitter emitter, Particle particle)
	{
		emitters[emitter.uniqueID].particle = particle.uniqueID;
	}

	void ParticleSystem::EmitterSetLocation(Emitter emitter, Vector2 location)
	{
		emitters[emitter.uniqueID].location = location;
	}

	void ParticleSystem::EmitterSetShape(Emitter emitter, EmitterShape shape)
	{
		emitters[emitter.uniqueID].shape = shape;
	}

	void ParticleSystem::EmitterSetSize(Emitter	emitter, Vector2 size)
	{
		emitters[emitter.uniqueID].size = size;
	}

	void ParticleSystem::EmitterSetFrequency(Emitter emitter, float frequency)
	{
		emitters[emitter.uniqueID].frequency = frequency;
	}

	unsigned ParticleSystem::GetParticleCount()
	{
		return numParticles;
	}

	Vector2 ParticleSystem::GetParticleLocation(unsigned particleIndex)
	{
		if (particleIndex > numParticles)
			return Vector2();

		return particles[particleIndex].location;
	}

	Color ParticleSystem::GetParticleColor(unsigned particleIndex)
	{
		if (particleIndex > numParticles)
			return Color();

		return particles[particleIndex].GetColor();
	}

	float ParticleSystem::GetParticleSize(unsigned particleIndex)
	{
		if (particleIndex > numParticles)
			return 0.f;

		return particles[particleIndex].size;
	}

	Vector2 ParticleSystem::GetParticleScale(unsigned particleIndex)
	{
		if (particleIndex > numParticles)
			return Vector2();

		return particles[particleIndex].data.scale;
	}

	// Private

	void ParticleSystem::addParticle(unsigned defIndex, Vector2 location)
	{
		unsigned slot = getFreeParticleSlot();
		particles[slot].Init(particleDefenitions[defIndex], location);
		numParticles++;
	}

	void ParticleSystem::removeParticle(unsigned particleIndex)
	{
		addFreeParticleSlot(particleIndex);

		ParticleData temp = particles[numParticles];
		numParticles--;
	}

	void ParticleSystem::addFreeDefinitionSlot(unsigned slot)
	{
		numFreeDefenitionSlots++;
		freeDefenitionSlots[numFreeDefenitionSlots] = slot;
	}

	void ParticleSystem::addFreeParticleSlot(unsigned slot)
	{
		numFreeDefenitionSlots++;
		freeParticleSlots[numFreeParticleSlots] = slot;
	}

	void ParticleSystem::addFreeEmitterSlot(unsigned slot)
	{
		numFreeDefenitionSlots++;
		freeEmitterSlots[numFreeEmitterSlots] = slot;
	}

	unsigned ParticleSystem::getFreeDefenitionSlot()
	{
		if (numFreeDefenitionSlots == 0)
			return numDefenitions;

		unsigned slot = freeDefenitionSlots[numFreeDefenitionSlots];
		numFreeDefenitionSlots--;
		return slot;
	}

	unsigned ParticleSystem::getFreeParticleSlot()
	{
		if (numFreeParticleSlots == 0)
			return numParticles;

		unsigned slot = freeParticleSlots[numFreeParticleSlots];
		numFreeParticleSlots--;
		return slot;
	}

	unsigned ParticleSystem::getFreeEmitterSlot()
	{
		if (numFreeEmitterSlots == 0)
			return numEmitters;

		unsigned slot = freeEmitterSlots[numFreeEmitterSlots];
		numFreeEmitterSlots--;
		return slot;
	}
};