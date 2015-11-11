// ParticleSystem.cpp

#include "ParticleSystem.h"

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

namespace PS
{
	/////////////////////////////////////////////////////////////////////
	// Particle
	/////////////////////////////////////////////////////////////////////

	void Particle::SetLifetime(float minLife, float maxLife)
	{
		owner->ParticleSetLifetime(*this, minLife, maxLife);
	}

	void Particle::SetSize(float sizeMin, float sizeMax, float sizeChange)
	{
		owner->ParticleSetSize(*this, sizeMin, sizeMax, sizeChange);
	}

	void Particle::SetRotation(float rotMin, float rotMax, float rotChange, bool rotRelative)
	{
		owner->ParticleSetRotation(*this, rotMin, rotMax, rotChange, rotRelative);
	}

	void Particle::SetScale(float scaleX, float scaleY)
	{
		owner->ParticleSetScale(*this, scaleX, scaleY);
	}

	void Particle::SetColor(Color color)
	{
		owner->ParticleSetColor(*this, color, color);
	}

	void Particle::SetColor(Color colorStart, Color colorEnd)
	{
		owner->ParticleSetColor(*this, colorStart, colorEnd);
	}

	void Particle::SetDirection(float dirMin, float dirMax, float dirChange)
	{
		owner->ParticleSetDirection(*this, dirMin, dirMax, dirChange);
	}

	void Particle::SetSpeed(float speedMin, float speedMax, float speedChange)
	{
		owner->ParticleSetSpeed(*this, speedMin, speedMax, speedChange);
	}

	void Particle::SetVelocity(Vector2 velocity)
	{
		owner->ParticleSetVelocity(*this, velocity);
	}

	void Particle::SetGravity(float direction, float strength)
	{
		owner->ParticleSetGravity(*this, direction, strength);
	}

	void Particle::SetAttractorPoint(Vector2 position, float strength)
	{
		owner->ParticleSetAttractorPoint(*this, position, strength);
	}

	void Particle::SetSpawnedParticle(Particle& spawnedParticle, unsigned numberOfSpawnedParticles)
	{
		owner->ParticleSetSpawnedParticle(*this, spawnedParticle, numberOfSpawnedParticles);
	}

	void Particle::SetCustomData(void* data)
	{
		owner->ParticleSetCustomData(*this, data);
	}

	// Private

	Particle::Particle()
	{
		Reset();
	}

	Particle::Particle(ParticleSystem* system)
	{
		Reset();

		owner = system;
	}

	void Particle::Reset()
	{
		owner = nullptr;
		uniqueID = (unsigned)-1;
		valid = false;
	}

	/////////////////////////////////////////////////////////////////////
	// Emitter
	/////////////////////////////////////////////////////////////////////

	void Emitter::SetLocation(Vector2 location)
	{
		owner->EmitterSetLocation(*this, location);
	}

	void Emitter::SetPoint(Vector2 location)
	{
		owner->EmitterSetPoint(*this, location);
	}

	void Emitter::SetCircle(float radius, Vector2 location)
	{
		owner->EmitterSetCircle(*this, radius, location);
	}

	void Emitter::SetRectangle(Vector2 dimension, Vector2 location)
	{
		owner->EmitterSetRectangle(*this, dimension, location);
	}

	void Emitter::SetRim(float thickness)
	{
		owner->EmitterSetRim(*this, thickness);
	}

	void Emitter::SetFrequency(float frequency, unsigned spawnCount, bool spawnImmediately)
	{
		owner->EmitterSetFrequency(*this, frequency, spawnCount, spawnImmediately);
	}

	void Emitter::Burst(unsigned spawnedParticlesOverride)
	{
		owner->EmitterBurst(*this, spawnedParticlesOverride);
	}

	void Emitter::SetActive(bool state)
	{
		owner->EmitterSetActive(*this, state);
	}

	// Private

	Emitter::Emitter()
	{
		Reset();
	}

	Emitter::Emitter(ParticleSystem* system)
	{
		Reset();

		owner = system;
	}

	void Emitter::Reset()
	{
		owner = nullptr;
		uniqueID = (unsigned)-1;
		valid = false;
	}

	/////////////////////////////////////////////////////////////////////
	// Vector2
	/////////////////////////////////////////////////////////////////////

	Vector2::Vector2()
		: X(0.0f)
		, Y(0.0f)
	{}

	Vector2 Vector2::CreateUnit(float direction)
	{
		float angle = direction * Math::degToRad;
		float x = cos(angle);
		float y = sin(angle);

		float hyp = sqrtf(x*x + y*y);
		if (hyp > 0.0f)
		{
			x /= hyp;
			y /= hyp;
		}

		return (Vector2(x, y));
	}

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

	float Vector2::Length()
	{
		return std::sqrt(X * X + Y * Y);
	}

	void Vector2::Normalize()
	{
		float len = Length();
		if (len > 0.0f)
		{
			float inverted = 1.0f / len;
			X *= inverted;
			Y *= inverted;
		}
	}

	float Vector2::Distance(Vector2& target)
	{
		Vector2 difference = target - *this;
		return (difference.Length());
	}

	/////////////////////////////////////////////////////////////////////
	// Color
	/////////////////////////////////////////////////////////////////////

	const Color Color::Black(0, 0, 0);
	const Color Color::White(255, 255, 255);
	const Color Color::Red(255, 0, 0);
	const Color Color::Green(0, 255, 0);
	const Color Color::Blue(0, 0, 255);
	const Color Color::Yellow(255, 255, 0);
	const Color Color::Magenta(255, 0, 255);
	const Color Color::Cyan(0, 255, 255);
	const Color Color::Transparent(0, 0, 0, 0);

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
		R = (float)C.R / 255.0;
		G = (float)C.G / 255.0;
		B = (float)C.B / 255.0;

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

		if (Hue < 0) { Hue = 360 - abs(Hue); }
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

	ParticleSystem::ParticleDef::ParticleDef()
	{
		particles = new ParticleOutput[MAX_PARTICLES];
		spawnedParticles = new Vector2[MAX_PARTICLES];
		emitters = new EmitterDef[MAX_EMITTERS];
		emitterActive = new bool[MAX_EMITTERS];

		Reset();
	}

	ParticleSystem::ParticleDef::~ParticleDef()
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

	unsigned ParticleSystem::ParticleDef::Reset()
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

		sizeMin = 32.0f;
		sizeMax = 32.0f;
		sizeChange = 0.0f;

		rotationRelative = false;
		rotationMin = 0.0f;
		rotationMax = 0.0f;
		rotationChange = 0.0f;

		dirMin = 0.0f;
		dirMax = 0.0f;
		dirChange = 0.0f;

		speedMin = 0.0f;
		speedMax = 0.0f;
		speedChange = 0.0f;

		velocity = Vector2(0.0f, 0.0f);
		gravity = Vector2(0.0f, 0.0f);

		colorDeltaH = 0.0f;
		colorDeltaS = 0.0f;
		colorDeltaL = 0.0f;
		colorDeltaA = 0.0f;
		colorStart = Color();
		colorEnd = Color();

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

	int ParticleSystem::ParticleDef::ProcessAll(float deltaTime, SpawnedParticleContainer& container)
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

	unsigned ParticleSystem::ParticleDef::Burst(unsigned emitterIndex, unsigned spawnedParticlesOverride)
	{
		unsigned Result = 0;

		unsigned numSpawnedBurstParts = 0;
		Vector2 numBursted[ParticleSystem::MAX_PARTICLES];

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

	void ParticleSystem::ParticleDef::SpawnParticle(Vector2 location)
	{
		if (numSpawnedParticles < MAX_PARTICLES)
		{
			spawnedParticles[numSpawnedParticles] = location;
			numSpawnedParticles++;
		}
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

	unsigned ParticleSystem::ParticleDef::GetEmitterCount()
	{
		return numEmitters;
	}

	EmitterDebugOutput ParticleSystem::ParticleDef::GetEmitter(unsigned emitterIndex)
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

	void ParticleSystem::ParticleDef::AddFlag(EParticleFlags flag)
	{
		flagBits |= flag;
	}

	void ParticleSystem::ParticleDef::RemoveFlag(EParticleFlags flag)
	{
		flagBits &= ~flag;
	}

	bool ParticleSystem::ParticleDef::HasFlag(EParticleFlags flag)
	{
		bool Result = (flagBits & flag ? true : false);
		return(Result);
	}

	void ParticleSystem::ParticleDef::CalcNewVelocityData()
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

	void ParticleSystem::ParticleDef::Process(ParticleOutput& output, float deltaTime)
	{
		//Size
		if (HasFlag(Flag_Size))
		{
			output.size = updateSize(output.size, deltaTime);
		}

		// Color
		if (HasFlag(Flag_Color))
			updateColor(output, deltaTime);

		// Velocity
		if (HasFlag(Flag_Velocity))
		{
			if (HasFlag(Flag_ConstVelocity))
			{
				output.location += output.locationData.Velocity * deltaTime;
				return;
			}

			bool shouldUpdateSpeed = HasFlag(Flag_Speed);
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

		if (HasFlag(Flag_Gravity))
			output.location += gravity * deltaTime;

		if (HasFlag(Flag_AttractorPoint))
			updateAttractor(output, deltaTime);
		
		// Rotation
		if (HasFlag(Flag_Rotation))
			updateRotation(output, deltaTime);
	}

	float ParticleSystem::ParticleDef::updateSize(float currentSize, float deltaTime)
	{
		float Result = currentSize + sizeChange*deltaTime;

		if (Result < 0.0f)
		{
			Result = 0.0f;
		}

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

	float ParticleSystem::ParticleDef::updateSpeed(float currentSpeed, float deltaTime)
	{
		float Result = currentSpeed + (speedChange * deltaTime);

		if (Result <= 0.0f)
			Result = 0.0f;

		return(Result);
	}

	float ParticleSystem::ParticleDef::updateDirection(float currentDirection, float deltaTime)
	{
		float Result = currentDirection + (dirChange * deltaTime);
		return(Result);
	}

	void ParticleSystem::ParticleDef::updateAttractor(ParticleOutput& output, float deltaTime)
	{
		Vector2 currentLocation = output.location;
		Vector2 difference = attractorPoint - currentLocation;
		difference.Normalize();

		output.location += difference * attractStrength * deltaTime;
	}

	void ParticleSystem::ParticleDef::updateRotation(ParticleOutput& output, float deltaTime)
	{
		if (rotationRelative)
			output.rotation = output.locationData.direction();
		else
		{
			output.rotation += rotationChange * deltaTime;
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
		location = Vector2(64, 64);
		shape = EmitterShape::POINT;

		dimension = Vector2(0.0f, 0.0f);
		rim = 0.0f;
		minRectRim = Vector2(0.0f, 0.0f);

		frequency = 1.0f;
		particleCount = 1;

		timer = 0.0f;
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
				Vector2 point;

				float minRadius = rim == 0.0f ? 0.0f : dimension.X - rim;

				float angle = Random::betweenf(0.f, PI_2);
				float radius = Random::betweenf(minRadius, dimension.X);

				point.X = location.X + radius * cos(angle);
				point.Y = location.Y + radius * sin(angle);

				return point;
			}
			break;
			case EmitterShape::RECTANGLE:
			{
				if (rim == 0.0f)
				{
					return location +
						Vector2((dimension.X*-1.0f)*0.5f + Random::random(dimension.X),
								(dimension.Y*-1.0f)*0.5f + Random::random(dimension.Y));
				}
				else
				{
					float xPos, yPos;
					unsigned side = Random::between(1, 4);
					switch (side)
					{
						// Top
					case 1:
						xPos = (dimension.X*-1.0f)*0.5f + Random::random(dimension.X);
						yPos = Random::betweenf(minRectRim.Y, dimension.Y * 0.5f) * -1.0f;
					break;
						// Right
					case 2:
						xPos = Random::betweenf(minRectRim.X, dimension.X * 0.5f);
						yPos = (dimension.Y*-1.0f)*0.5f + Random::random(dimension.Y);
					break;
						// Down
					case 3:
						xPos = (dimension.X*-1.0f)*0.5f + Random::random(dimension.X);
						yPos = Random::betweenf(minRectRim.Y, dimension.Y * 0.5f);
					break;
						// Left
					case 4:
						xPos = Random::betweenf(minRectRim.X, dimension.X * 0.5f) * -1.0f;
						yPos = (dimension.Y*-1.0f)*0.5f + Random::random(dimension.Y);
					break;
					}

					return location + Vector2(xPos, yPos);
				}
			}
			break;
		}

		return Vector2();
	}

	void ParticleSystem::EmitterDef::InitTimer()
	{
		timer = frequency;
	}

	void ParticleSystem::EmitterDef::CalcRectRim()
	{
		if (rim == 0.0f)
			return;

		minRectRim.X = (dimension.X * 0.5f) - rim;
		minRectRim.Y = (dimension.Y * 0.5f) - rim;
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
		numEmitters = 0;
	}

	ParticleSystem::~ParticleSystem()
	{
		delete[] particleDefinitions;
		particleDefinitions = nullptr;
	}

	void ParticleSystem::Update(float deltaTime)
	{
		SpawnedParticleContainer spawnedParticles[MAX_DEFINITIONS];

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
		handle.valid = (numDefinitions < MAX_DEFINITIONS);
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
		handle.valid = (spawnedParticle.valid && currentEmitterCount < MAX_EMITTERS);
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

		ParticleDef& def = particleDefinitions[particle.uniqueID];
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

	// Private

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

	void ParticleSystem::ParticleSetSize(Particle& particle, float sizeMin, float sizeMax, float sizeChange)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.sizeMin = std::min(sizeMin, sizeMax);
		def.sizeMax = std::max(sizeMin, sizeMax);
		def.sizeChange = sizeChange;

		if (sizeChange != 0.0f)
		{
			def.AddFlag(Flag_Size);
		}
	}

	void ParticleSystem::ParticleSetRotation(Particle& particle, float rotMin, float rotMax, float rotChange, bool rotRelative)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.rotationMin = std::min(rotMin, rotMax);
		def.rotationMax = std::max(rotMin, rotMax);
		def.rotationChange = rotChange;
		def.rotationRelative = rotRelative;

		if (rotChange != 0.0f)
		{
			def.AddFlag(Flag_Rotation);
		}
	}

	void ParticleSystem::ParticleSetScale(Particle& particle, float scaleX, float scaleY)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.scale = Vector2(scaleX, scaleY);
	}

	void ParticleSystem::ParticleSetColor(Particle& particle, Color colorStart, Color colorEnd)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.colorStart = HSL(colorStart);
		def.colorEnd = HSL(colorEnd);
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

			def.AddFlag(Flag_HSL);
			def.AddFlag(Flag_Color);
		}
		else
		{
			def.RemoveFlag(Flag_HSL);
		}

		if (colorStart.A != colorEnd.A)
		{
			def.colorDeltaA = (def.colorStartAlpha - def.colorEndAlpha) * -1.0f;

			def.AddFlag(Flag_Color);
		}
		else
		{
			def.colorDeltaA = 0.f;
		}
	}

	void ParticleSystem::ParticleSetDirection(Particle& particle, float dirMin, float dirMax, float dirChange)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.dirMin = std::min(dirMin, dirMax);
		def.dirMax = std::max(dirMin, dirMax);
		def.dirChange = dirChange;

		def.CalcNewVelocityData();
	}

	void ParticleSystem::ParticleSetSpeed(Particle& particle, float speedMin, float speedMax, float speedChange)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.speedMin = std::min(speedMin, speedMax);
		def.speedMax = std::max(speedMin, speedMax);
		def.speedChange = speedChange;

		def.CalcNewVelocityData();
	}

	void ParticleSystem::ParticleSetVelocity(Particle& particle, Vector2 velocity)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.velocity = velocity;

		if (velocity != Vector2(0.0f, 0.0f))
		{
			def.AddFlag(Flag_Velocity);
			def.AddFlag(Flag_GlobalVelocity);
			def.RemoveFlag(Flag_Speed);
			def.RemoveFlag(Flag_Direction);
		}
	}

	void ParticleSystem::ParticleSetGravity(Particle& particle, float direction, float strength)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.gravity = Vector2::CreateUnit(direction) * strength;

		if (def.gravity != Vector2(0.0f, 0.0f))
		{
			def.AddFlag(Flag_Gravity);
		}
	}

	void ParticleSystem::ParticleSetAttractorPoint(Particle& particle, Vector2 location, float strength)
	{
		if (particle.valid == false)
			return;

		ParticleDef& def = particleDefinitions[particle.uniqueID];
		def.attractorPoint = location;
		def.attractStrength = strength;

		if (strength != 0.0f)
		{
			def.AddFlag(Flag_AttractorPoint);
		}
		else
		{
			def.RemoveFlag(Flag_AttractorPoint);
		}
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
		def.shape = EmitterShape::POINT;
		def.location = location;
	}

	void ParticleSystem::EmitterSetCircle(Emitter emitter, float radius, Vector2 location)
	{
		if (emitter.valid == false)
			return;

		EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];
		def.shape = EmitterShape::CIRCLE;
		def.location = location;
		def.dimension = Vector2(radius, radius);
	}

	void ParticleSystem::EmitterSetRectangle(Emitter emitter, Vector2 dimension, Vector2 location)
	{
		if (emitter.valid == false)
			return;

		EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];

		def.shape = EmitterShape::RECTANGLE;
		def.location = location;
		def.dimension = dimension;

		def.CalcRectRim();
	}

	void ParticleSystem::EmitterSetRim(Emitter emitter, float thickness)
	{
		if (emitter.valid == false)
			return;

		EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];

		def.rim = thickness;

		def.CalcRectRim();
	}

	void ParticleSystem::EmitterSetFrequency(Emitter emitter, float frequency, unsigned spawnCount, bool spawnImmediately)
	{
		if (emitter.valid == false)
			return;

		EmitterDef& def = particleDefinitions[emitter.particleID].emitters[emitter.uniqueID];
		def.frequency = frequency;
		def.particleCount = spawnCount;
		if (spawnImmediately)
		{
			def.InitTimer();
		}
	}

	void ParticleSystem::EmitterBurst(Emitter emitter, unsigned spawnedParticlesOverride)
	{
		if (emitter.valid == false)
			return;

		numParticles += particleDefinitions[emitter.particleID].Burst
			(emitter.uniqueID, spawnedParticlesOverride);
	}

	void ParticleSystem::EmitterSetActive(Emitter emitter, bool state)
	{
		ParticleDef& def = particleDefinitions[emitter.particleID];

		def.emitterActive[emitter.uniqueID] = state;
	}

	ParticleSystem::ParticleDef* ParticleSystem::getDefinitionFromIndexParticles(unsigned& index)
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

	ParticleSystem::ParticleDef* ParticleSystem::getDefenitionFromIndexEmitters(unsigned& index)
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

	ParticleSystem::ParticleOutput* ParticleSystem::GetParticle(unsigned particleIndex)
	{
		if (particleIndex > particleDefinitions[0].numParticles)
			return &particleDefinitions[0].particles[particleDefinitions[0].numParticles];

		return &particleDefinitions[0].particles[particleIndex];
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
			currentDef = partSystem->getDefinitionFromIndexParticles(defIndex);
			if (currentDef != nullptr)
			{
				numParticlesInDef = currentDef->GetParticleCount() - 1;
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
			currentDef = partSystem->getDefinitionFromIndexParticles(defIndex);
			if (currentDef == nullptr)
			{
				reachedEnd = true;

				return;
			}
			else
			{
				particleIndex = 0;
				numParticlesInDef = currentDef->GetParticleCount() - 1;
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
	// EmitterDebugIterator
	/////////////////////////////////////////////////////////////////////

	EmitterDebugIterator::EmitterDebugIterator(class ParticleSystem& particleSystem)
	{
		partSystem = &particleSystem;

		numDefinitions = partSystem->numDefinitions;
		defIndex = 0;
		numEmittersInDef = 0;
		emitterIndex = 0;

		indexCounter = 0;

		currentDef = nullptr;

		reachedEnd = partSystem->GetEmitterCount() == 0 ? true : false;

		if (reachedEnd == false)
		{
			currentDef = partSystem->getDefenitionFromIndexEmitters(defIndex);
			if (currentDef != nullptr)
			{
				numEmittersInDef = currentDef->GetEmitterCount() - 1;
				target = currentDef->GetEmitter(emitterIndex);
			}
			else
			{
				reachedEnd = true;
			}
		}
	}

	unsigned EmitterDebugIterator::CurrentIndex()
	{
		return indexCounter;
	}

	void EmitterDebugIterator::operator++()
	{
		indexCounter++;
		if (emitterIndex < numEmittersInDef)
		{
			emitterIndex++;
			target = currentDef->GetEmitter(emitterIndex);

			return;
		}
		else
		{
			defIndex++;
			currentDef = partSystem->getDefenitionFromIndexEmitters(defIndex);
			if (currentDef == nullptr)
			{
				reachedEnd = true;

				return;
			}
			else
			{
				emitterIndex = 0;
				numEmittersInDef = currentDef->GetEmitterCount() - 1;
				target = currentDef->GetEmitter(emitterIndex);

				return;
			}
		}
	}

	void EmitterDebugIterator::operator++(int)
	{
		++(*this);
	}

	const EmitterDebugOutput EmitterDebugIterator::operator*() const
	{
		return target;
	}

	const EmitterDebugOutput EmitterDebugIterator::operator->() const
	{
		return target;
	}

	EmitterDebugIterator::operator bool() const
	{
		return !reachedEnd;
	}
};