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

		Vector2 operator*(float value);

		friend bool operator!=(Vector2& first, Vector2& second);

		float length();
		void normalize();

		float X, Y;
	};

	struct Color
	{
		Color();
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

		friend bool operator!=(Color& first, Color& second);

		unsigned char R, G, B, A;

	private:

		void clampColor();
		unsigned char clampValue(unsigned char value);
	};

	struct HSL
	{
		HSL();
		HSL(int H, int S, int L);
		HSL(const Color& C);

		Color TurnToRGB();

		friend bool operator!=(HSL& first, HSL& second);

		static double EPSILON;

		double Hue;
		double Saturation;
		double Luminance;

	private:

		double HueToRGB(double arg1, double arg2, double H);

	};


	class ParticleSystem
	{
	public:

		struct ParticleOutput
		{
			friend class ParticleSystem;
			friend class ParticleDef;

			struct LocationData
			{
				Vector2 Velocity;
				/*float direction;
				float speed;*/

				float& speed() { return Velocity.X; }
				float& direction() { return Velocity.Y; }

				LocationData()
					/*: direction(0.0f)
					, speed(0.0f)*/
				{}
			};

			struct ColorData
			{
				float H, S, L, A;
				float deltaFactor;

				void Set(HSL hsl, float a)
				{
					H = (float)hsl.Hue;
					S = (float)hsl.Saturation;
					L = (float)hsl.Luminance;
					A = a;
				}
			};

		public:
			Vector2		location;
			Vector2		scale;
			float		rotation;
			float		size;
			Color		color;
			void*		customData;

			ParticleOutput()
				: scale(1.0f, 1.0f)
				, rotation(0.0f)
				, size(1.0f)
				, m_def(-1)
			{
				m_locationData = LocationData();
				m_colorData = ColorData();
			}

			~ParticleOutput()
			{
			}

		private:

			unsigned m_def;
			float m_life;

			LocationData m_locationData;
			ColorData m_colorData;
		};

	private:

		enum EParticleFlags
		{
			Flag_Location	= 0x0001, // 1
			Flag_Speed		= 0x0002, // 2
			Flag_Direction	= 0x0004, // 3
			Flag_ConstVel	= 0x0008, // 4
			Flag_Size		= 0x0010, // 5
			Flag_Rotation	= 000020, // 6
			Flag_Color		= 0x0040, // 7
			Flag_HSL		= 0x0080, // 8
			Flag_Flag9		= 0x0100, // 9
			Flag_Flag10		= 0x0200, // 10
			Flag_Flag11		= 0x0400, // 11
			Flag_Flag12		= 0x0800, // 12
			Flag_Flag13		= 0x1000, // 13
			Flag_Flag14		= 0x2000, // 14
			Flag_Flag15		= 0x4000, // 15
			Flag_Flag16		= 0x8000  // 16
		};

		class ParticleDef
		{
			friend class ParticleSystem;
		public:
			
			ParticleDef();

			void Reset();
			void Process(ParticleOutput& output, float deltaTime);

		public:
			
			float minLife, maxLife;
			
			Vector2 scale;
			
			// Size
			float sizeMin, sizeMax;
			float sizeInc;
			float sizeWiggle;

			// Color
			float colorDeltaH, colorDeltaS, colorDeltaL, colorDeltaA;
			float colorStartAlpha, colorEndAlpha;
			HSL colorStart, colorEnd;

			// Rotation
			bool rotationRelative;
			float rotationMin, rotationMax;
			float rotationInc;
			float rotationWiggle;

			// Speed
			float speedMin, speedMax;
			float speedInc;
			float speedWiggle;

			// Direction
			float dirMin, dirMax;
			float dirInc;
			float dirWiggle;

			Vector2 Velocity;

			unsigned particle;
			void* customData;

		private:

			float updateSize(float currentSize, float deltaTime);
			void updateColor(ParticleOutput& output, float deltaTime);
			float updateSpeed(float currentSpeed, float deltaTime);
			float updateDirection(float currentDirection, float deltaTime);
			void updateVelocity(float currentSpeed, float currentDirection);
			void updateRotation(ParticleOutput& output, float deltaTime);

		private:

			static const float degToRad;

			unsigned short m_flagBits;
		};

		class Emitterdef
		{
		public:

			enum class EmitterShape
			{
				POINT,
				CIRCLE,
				RECTANGLE,
			};

			Emitterdef();

			void Reset();
			bool Update(float deltaTime);
			Vector2 GetSpawnLocation();

		public:

			Vector2 location;
			EmitterShape shape;
			
			unsigned particle;

			float frequency;
			unsigned particleCount;

			Vector2 point;
			Vector2 dims;

		private:

			static const float PI_2;

			float timer;
		};

	public:

		ParticleSystem();
		~ParticleSystem();

		void Update(float deltaTime);

		Particle CreateParticle();
		Emitter CreateEmitter();
		void DestroyParticle(Particle particle);
		void DestroyEmitter(Emitter emitter);

		void SpawnParticle(Particle particle, Vector2 location);

		void ClearVisibleParticles();

		void EmitterSetParticle(Emitter emitter, Particle particle, unsigned spawnCount = 1);
		void EmitterSetLocation(Emitter emitter, Vector2 location);
		void EmitterSetPoint(Emitter emitter, Vector2 location);
		void EmitterSetCircle(Emitter emitter, Vector2 location, float radius);
		void EmitterSetRectangle(Emitter emitter, Vector2 location, Vector2 dimension);
		void EmitterSetFrequency(Emitter emitter, float frequency);

		void ParticleSetLifetime(Particle particle, float minLife, float maxLife);
		void ParticleSetSize(Particle particle, float sizeMin, float sizeMax, float sizeInc = 0.0f, float sizeWiggle = 0.0f);
		void ParticleSetRotation(Particle particle, float rotMin, float rotMax, float rotInc = 0.0f, float rotWiggle = 0.0f, bool rotRelative = false);
		void ParticleSetScale(Particle particle, float scaleX, float scaleY);
		void ParticleSetColor(Particle particle, Color color);
		void ParticleSetColor(Particle particle, Color colorStart, Color colorEnd);
		void ParticleSetDirection(Particle particle, float dirMin, float dirMax, float dirInc = 0.0f, float dirWiggle = 0.0f);
		void ParticleSetSpeed(Particle particle, float speedMin, float speedMax, float speedInc = 0.0f, float speedWiggle = 0.0f);
		void ParticleSetVelocity(Particle particle, Vector2 velocity);
		void ParticleSetSpawnedParticle(Particle particle, Particle spawnedParticle);
		void ParticleSetCustomData(Particle particle, void* data);

		unsigned GetParticleCount();
		ParticleOutput* GetParticle(unsigned ParticleIndex);

	private:

		void addParticle(unsigned defIndex, Vector2 location);
		void removeParticle(unsigned particleIndex);

		void initParticle(ParticleOutput& info, unsigned defIndex);

#if 0
		void addFreeDefinitionSlot(unsigned slot);
		void addFreeParticleSlot(unsigned slot);
		void addFreeEmitterSlot(unsigned slot);
		unsigned getFreeDefenitionSlot();
		unsigned getFreeEmitterSlot();
		unsigned getFreeParticleSlot();
#endif

	private:

		static const int MAX_PARTICLE_DEFS = 100;
		unsigned numDefinitions;
		ParticleDef* particleDefinitions;
		
		/*unsigned numFreeDefinitionSlots;
		unsigned freeDefinitionSlots[MAX_PARTICLE_DEFS];*/

		unsigned particleDefCacheIndex;
		ParticleDef particleDefCache;

		static const int MAX_EMITTERS = 100;
		unsigned numEmitters;
		Emitterdef* emitters/*[MAX_EMITTERS]*/;
		/*unsigned numFreeEmitterSlots;
		unsigned freeEmitterSlots[MAX_EMITTERS];*/

		static const int MAX_PARTICLES = 1000;
		unsigned numParticles;
		ParticleOutput* particles;
		/*unsigned numFreeParticleSlots;
		unsigned freeParticleSlots[MAX_PARTICLES];*/
	};

	typedef ParticleSystem::ParticleOutput Output;
};