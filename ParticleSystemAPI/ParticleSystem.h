// ParticleSystem.h

#pragma once

////////////////////////////////////////////////////////////
//
//  "Graphics API indepenent Particle System Implementation"
//  Per "Gimmic" Johansson (http://codergimmic.wordpress.com)
//
////////////////////////////////////////////////////////////

namespace PS
{
	class Particle
	{
		friend class ParticleSystem;
		unsigned uniqueID;
	};

	class Emitter
	{
		friend class ParticleSystem;
		unsigned uniqueID;
	};

	enum class EmitterShape
	{
		POINT,
		CIRCLE,
		RECTANGLE,
	};

	struct Vector2
	{
		Vector2();
		Vector2(float x, float y);

		Vector2& operator=(const Vector2& rhs);
		Vector2& operator+=(const Vector2& rhs);
		Vector2& operator-=(const Vector2& rhs);

		Vector2& operator*=(const float value);
		Vector2& operator/=(const float value);

		Vector2 operator+(const Vector2& rhs);
		Vector2 operator-(const Vector2& rhs);
		Vector2 operator*(const Vector2& rhs);
		Vector2 operator/(const Vector2& rhs);

		float length();
		void normalize();

		float X, Y;
	};

	struct Color
	{
		Color();
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

		unsigned char R, G, B, A;

	private:

		void clampColor();
		unsigned char clampValue(unsigned char value);
	};

	struct HSL
	{
		HSL();
		HSL(int H, int S, int L);

		Color TurnToRGB();

		static double EPSILON;

		double Hue;
		double Saturation;
		double Luminance;

	private:

		double HueToRGB(double arg1, double arg2, double H);

	};

	HSL TurnToHSL(const Color& C);

	class ParticleSystem
	{
		class ParticleDef
		{
		public:
			ParticleDef();

			float minLife, maxLife;
			
			Vector2 scale;
			
			// Size
			bool sizeConst;
			float size;
			float sizeMin, sizeMax;
			float sizeInc;
			float sizeWiggle;

			//Rotation
			bool rotationRelative;
			bool rotationConst;
			float rotation;
			float rotationMin, rotationMax;
			float rotationInc;
			float rotationWiggle;

			// Direction
			bool directionConst;
			float direction;
			float directionMin, directionMax;
			float directionInc;
			float directionWiggle;

			// Speed
			bool speedConst;
			float speed;
			float speedMin, speedMax;
			float speedInc;
			float speedWiggle;

			Vector2 Velocity;

			// Color
			float colorH, colorS, colorL, colorA;
			float colorDeltaH, colorDeltaS, colordeltaL, colorDeltaA;
			Color colorStart, colorEnd;

			unsigned particle;
			void* customData;
		};

		class Emitterdef
		{
		public:
			Emitterdef();
			Emitterdef(EmitterShape newShape);

			bool Update(float deltaTime);

			void SetFrequency(float frequency);

			void SetLocation(Vector2 newLocation);
			
			Vector2 GetSpawnLocation();

		public:

			Vector2 location;
			EmitterShape shape;
			
			unsigned particle;

			float frequency;
			
			Vector2 size;

		private:

			float timer;
		};

		struct ParticleData
		{
		public:
			ParticleData();

			void Init(ParticleDef def, Vector2 newLocation);

			bool Update(float deltaTime);
			Color GetColor();

		public:

			Vector2 location;
			float lifeLeft;
			ParticleDef data;

			// Size
			bool sizeConst;
			float size;

			//Rotation
			bool rotationRelative;
			bool rotationConst;
			float rotation;

			// Direction
			bool directionConst;
			float direction;

			// Speed
			bool speedConst;
			float speed;

			Vector2 Velocity;

			// Color
			float colorH, colorS, colorL, colorA;
		};

	public:

		ParticleSystem();

		void Update(float deltaTime);

		Particle CreateParticle();
		Emitter CreateEmitter();
		void DestroyParticle(Particle particle);
		void DestroyEmitter(Emitter emitter);

		void ParticleSetLifetime(Particle particle, float minLife, float maxLife);
		void ParticleSetColor(Particle particle, Color color);
		void ParticleSetColor(Particle particle, Color colorStart, Color colorEnd);
		void ParticleSetVelocity(Particle particle, Vector2 velocity);
		void ParticleSetCustomData(Particle particle, void* data);

		void EmitterSetLocation(Emitter emitter, Vector2 location);
		void EmitterSetShape(Emitter emitter, EmitterShape shape);
		void EmitterSetSize(Emitter emitter, Vector2 size);

		unsigned GetParticleCount();
		Vector2 GetParticleLocation(unsigned particleIndex);
		Color GetParticleColor(unsigned particleIndex);

	private:

		void addParticle(unsigned defIndex, Vector2 location);
		void removeParticle(unsigned particleIndex);

		void addFreeDefinitionSlot(unsigned slot);
		void addFreeParticleSlot(unsigned slot);
		void addFreeEmitterSlot(unsigned slot);
		unsigned getFreeDefenitionSlot();
		unsigned getFreeParticleSlot();
		unsigned getFreeEmitterSlot();

	private:

		static const unsigned int MAX_PARTICLE_DEFS = 1000;
		unsigned numDefenitions;
		ParticleDef particleDefenitions[MAX_PARTICLE_DEFS];
		unsigned numFreeDefenitionSlots;
		unsigned freeDefenitionSlots[MAX_PARTICLE_DEFS];

		static const unsigned int MAX_PARTICLES = 10000;
		unsigned numParticles;
		ParticleData particles[MAX_PARTICLES];
		unsigned numFreeParticleSlots;
		unsigned freeParticleSlots[MAX_PARTICLES];

		static const unsigned int MAX_EMITTERS = 1000;
		unsigned numEmitters;
		Emitterdef emitters[MAX_EMITTERS];
		unsigned numFreeEmitterSlots;
		unsigned freeEmitterSlots[MAX_EMITTERS];
	};
};