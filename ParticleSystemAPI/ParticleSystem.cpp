// ParticleSystem.cpp

#include "ParticleSystem.h"

#include <cmath>
#include <algorithm>

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
	// ParticleDef
	/////////////////////////////////////////////////////////////////////

	ParticleSystem::ParticleDef::ParticleDef()
	{
		particle = (unsigned)-1;
	}

	/////////////////////////////////////////////////////////////////////
	// EmitterDef
	/////////////////////////////////////////////////////////////////////

	ParticleSystem::Emitterdef::Emitterdef()
	{
		particle = (unsigned)-1;
	}

	ParticleSystem::Emitterdef::Emitterdef(EmitterShape newShape)
	{
		shape = newShape;
	}

	bool ParticleSystem::Emitterdef::Update(float deltaTime)
	{
		timer += deltaTime;
		if (timer > frequency)
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
	}

	void ParticleSystem::ParticleData::Init(ParticleDef def, Vector2 newLocation)
	{
		data = def;
		location = newLocation;

	}

	bool ParticleSystem::ParticleData::Update(float deltaTime)
	{
		lifeLeft -= deltaTime;
		if (lifeLeft <= 0)
		{
			return true;
		}

		//TODO(Per): Add update code; color, position, size, rotation etc.

		return false;
	}

	Color ParticleSystem::ParticleData::GetColor()
	{
		Color ReturnColor = HSL((unsigned)colorH, (unsigned)colorS, (unsigned)colorL).TurnToRGB();
		ReturnColor.A = (unsigned char)colorA;
		return ReturnColor;
	}

	/////////////////////////////////////////////////////////////////////
	// ParticleSystem
	/////////////////////////////////////////////////////////////////////

	ParticleSystem::ParticleSystem()
	{
		
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
					numCreatedParticles++;
					createdParticles[numCreatedParticles] = particles[i].data.particle;
					createdParticleLocations[numCreatedParticles] = particles[i].location;
				}
			}
		}

		for (unsigned i = 0; i < numEmitters; i++)
		{
			if (emitters[i].Update(deltaTime))
			{
				numCreatedParticles++;
				createdParticles[numCreatedParticles] = emitters[i].particle;
				createdParticleLocations[numCreatedParticles] = emitters[i].GetSpawnLocation();
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
		return handle;
	}

	Emitter ParticleSystem::CreateEmitter()
	{
		Emitter handle;
		handle.uniqueID = getFreeEmitterSlot();
		return handle;
	}

	void ParticleSystem::DestroyParticle(Particle particle)
	{
		addFreeDefinitionSlot(particle.uniqueID);
	}

	void ParticleSystem::DestroyEmitter(Emitter emitter)
	{
		addFreeEmitterSlot(emitter.uniqueID);
	}

	void ParticleSystem::ParticleSetLifetime(Particle particle, float minLife, float maxLife)
	{
		ParticleDef& def = particleDefenitions[particle.uniqueID];
		def.minLife = minLife;
		def.maxLife = maxLife;
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

	void ParticleSystem::EmitterSetLocation(Emitter emitter, Vector2 location)
	{
		emitters[emitter.uniqueID].location = location;
	}

	void ParticleSystem::EmitterSetShape(Emitter emitter, EmitterShape shape)
	{
		emitters[emitter.uniqueID].shape = shape;
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

	// Private

	void ParticleSystem::addParticle(unsigned defIndex, Vector2 location)
	{
		unsigned slot = getFreeParticleSlot();
		particles[slot].Init(particleDefenitions[defIndex], location);
	}

	void ParticleSystem::removeParticle(unsigned particleIndex)
	{
		addFreeParticleSlot(particleIndex);

		ParticleData temp = particles[numParticles];
		
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