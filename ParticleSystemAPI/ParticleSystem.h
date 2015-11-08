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
	class ParticleSystem;
	struct Vector2;
	struct Color;

	/////////////////////////////////////////////////////////////////////
	// Particle
	/////////////////////////////////////////////////////////////////////

	class Particle
	{
	public:
		friend class ParticleSystem;

		void SetLifetime(float minLife, float maxLife);
		void SetSize(float sizeMin, float sizeMax, float sizeInc = 0.0f);
		void SetRotation(float rotMin, float rotMax, float rotInc = 0.0f, bool rotRelative = false);
		void SetScale(float scaleX, float scaleY);
		void SetColor(Color color);
		void SetColor(Color colorStart, Color colorEnd);
		void SetDirection(float dirMin, float dirMax, float dirInc = 0.0f);
		void SetSpeed(float speedMin, float speedMax, float speedInc = 0.0f);
		void SetVelocity(Vector2 velocity);
		void SetSpawnedParticle(Particle& spawnedParticle, unsigned numberOfSpawnedParticles = 1);
		void SetCustomData(void* data);

	private:

		Particle::Particle();
		Particle::Particle(ParticleSystem* system);

		void Reset();

	private:

		unsigned uniqueID;
		bool valid;

		ParticleSystem* owner;
	};

	/////////////////////////////////////////////////////////////////////
	// Emitter
	/////////////////////////////////////////////////////////////////////

	class Emitter
	{
	public:
		friend class ParticleSystem;

		void SetLocation(Vector2 location);
		void SetPoint(Vector2 location);
		void SetCircle(float radius, Vector2 location);
		void SetRectangle(Vector2 dimension, Vector2 location);
		void SetFrequency(float frequency, unsigned spawnCount = 1, bool spawnImmediately = false);
		void Burst(unsigned spawnedParticlesOverride = (unsigned)-1);
		void SetActive(bool state);

	private:

		Emitter::Emitter();
		Emitter::Emitter(ParticleSystem* system);

		void Reset();

	private:

		unsigned uniqueID;
		unsigned particleID;

		bool valid;

		ParticleSystem* owner;
	};

	/////////////////////////////////////////////////////////////////////
	// Vector2
	/////////////////////////////////////////////////////////////////////

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

	/////////////////////////////////////////////////////////////////////
	// Color
	/////////////////////////////////////////////////////////////////////

	struct Color
	{
		Color();
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

		friend bool operator!=(Color& first, Color& second);

	public:

		unsigned char R, G, B, A;

		static const Color Black;       ///< Black predefined color
		static const Color White;       ///< White predefined color
		static const Color Red;         ///< Red predefined color
		static const Color Green;       ///< Green predefined color
		static const Color Blue;        ///< Blue predefined color
		static const Color Yellow;      ///< Yellow predefined color
		static const Color Magenta;     ///< Magenta predefined color
		static const Color Cyan;        ///< Cyan predefined color
		static const Color Transparent; ///< Transparent (black) predefined color

	private:

		void clampColor();
		unsigned char clampValue(unsigned char value);
	};

	/////////////////////////////////////////////////////////////////////
	// HLS
	/////////////////////////////////////////////////////////////////////

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

	enum class EmitterShape
	{
		POINT,
		CIRCLE,
		RECTANGLE,
	};

	/////////////////////////////////////////////////////////////////////
	// ParticleSystem
	/////////////////////////////////////////////////////////////////////

	class ParticleSystem
	{
		static const unsigned MAX_DEFINITIONS = 100;
		static const unsigned MAX_PARTICLES = 10000;
		static const unsigned MAX_EMITTERS = 100;
		static const unsigned MAX_SPAWNCOUNT = 50;

		struct SpawnedParticleContainer
		{
			Vector2 locations[MAX_SPAWNCOUNT];
			unsigned size;

			SpawnedParticleContainer::SpawnedParticleContainer()
			{
				size = 0;
			}

			bool Add(Vector2 location)
			{
				if (size >= MAX_SPAWNCOUNT)
					return false;

				locations[size] = location;
				size++;

				return true;
			}
		};

	public:

		friend class ParticleIterator;
		friend class EmitterDebugIterator;
		friend class EmitterDebugOutput;
		friend class Particle;
		friend class Emitter;

		struct ParticleOutput
		{
			friend class ParticleSystem;
			friend class ParticleDef;

			struct LocationData
			{
				Vector2 Velocity;

				float& speed() { return Velocity.X; }
				float& direction() { return Velocity.Y; }
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
				, locationData()
				, colorData()
			{}

			~ParticleOutput()
			{}

		private:

			float lifeRemaining;

			LocationData locationData;
			ColorData colorData;
		};

	private:

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
			Flag_Flag10			= 0x0200, // 10
			Flag_Flag11			= 0x0400, // 11
			Flag_Flag12			= 0x0800, // 12
			Flag_Flag13			= 0x1000, // 13
			Flag_Flag14			= 0x2000, // 14
			Flag_Flag15			= 0x4000, // 15
			Flag_Flag16			= 0x8000  // 16
		};

		class EmitterDef;

		class ParticleDef
		{
			friend class ParticleSystem;

		public:
			
			ParticleDef();
			~ParticleDef();

			unsigned Reset();
			int ProcessAll(float deltaTime, SpawnedParticleContainer& container);

			unsigned Burst(unsigned emitterIndex, unsigned spawnedParticlesOverride = (unsigned)-1);
			void SpawnParticle(Vector2 location);

			unsigned GetParticleCount();
			ParticleOutput* GetParticle(unsigned particleindex);
			unsigned GetEmitterCount();
			class EmitterDebugOutput GetEmitter(unsigned emitterIndex);

			void AddFlag(EParticleFlags flag);
			void RemoveFlag(EParticleFlags flag);
			bool HasFlag(EParticleFlags flag);

			void CalcNewVelocityData();

		public:
			
			float minLife, maxLife;
			
			Vector2 scale;
			
			// Size
			float sizeMin, sizeMax;
			float sizeInc;

			// Color
			float colorDeltaH, colorDeltaS, colorDeltaL, colorDeltaA;
			float colorStartAlpha, colorEndAlpha;
			HSL colorStart, colorEnd;

			// Rotation
			bool rotationRelative;
			float rotationMin, rotationMax;
			float rotationInc;

			// Speed
			float speedMin, speedMax;
			float speedInc;

			// Direction
			float dirMin, dirMax;
			float dirInc;

			Vector2 Velocity;

			unsigned particle;
			unsigned particleSpawnCount;
			void* customData;

		private:

			void Process(ParticleOutput& output, float deltaTime);

			float updateSize(float currentSize, float deltaTime);
			void updateColor(ParticleOutput& output, float deltaTime);
			float updateSpeed(float currentSpeed, float deltaTime);
			float updateDirection(float currentDirection, float deltaTime);
			Vector2 updateVelocity(float currentSpeed, float currentDirection);
			void updateRotation(ParticleOutput& output, float deltaTime);

			bool addParticle(Vector2 location);
			bool removeParticle(unsigned particleIndex);

			void initParticle(ParticleOutput& info);

		private:

			static const float degToRad;

			unsigned short flagBits;

			unsigned numParticles;
			ParticleOutput* particles;
			unsigned numSpawnedParticles;
			Vector2* spawnedParticles;

			unsigned numEmitters;
			EmitterDef* emitters;
			bool* emitterActive;
		};

		class EmitterDef
		{
		public:

			EmitterDef();

			void Reset();
			unsigned Update(float deltaTime);
			Vector2 GetSpawnLocation();
			void InitTimer();

		public:

			Vector2 location;
			EmitterShape shape;

			float frequency;
			unsigned particleCount;

			Vector2 point;
			Vector2 dimension;

		private:

			static const float PI_2;

			float timer;
		};

	public:

		ParticleSystem();
		~ParticleSystem();

		void Update(float deltaTime);

		Particle CreateParticle();
		Emitter CreateEmitter(Particle spawnedParticle);
		void DestroyParticle(Particle& particle);
		void DestroyEmitter(Emitter emitter);

		void SpawnParticle(Particle particle, Vector2 location, unsigned spawnCount = 1);

		void ClearVisibleParticles();
		void ClearVisibleParticlesOfType(Particle& particle);

		unsigned GetParticleTypeCount();
		unsigned GetSpawnedParticleCountOfType(unsigned particle);
		unsigned GetSpawnedParticleCount();

		unsigned GetEmitterTypeCount(unsigned particle);
		unsigned GetEmitterCount();

	private:

		void ParticleSetLifetime(Particle& particle, float minLife, float maxLife);
		void ParticleSetSize(Particle& particle, float sizeMin, float sizeMax, float sizeInc = 0.0f);
		void ParticleSetRotation(Particle& particle, float rotMin, float rotMax, float rotInc = 0.0f, bool rotRelative = false);
		void ParticleSetScale(Particle& particle, float scaleX, float scaleY);
		void ParticleSetColor(Particle& particle, Color colorStart, Color colorEnd);
		void ParticleSetDirection(Particle& particle, float dirMin, float dirMax, float dirInc = 0.0f);
		void ParticleSetSpeed(Particle& particle, float speedMin, float speedMax, float speedInc = 0.0f);
		void ParticleSetVelocity(Particle& particle, Vector2 velocity);
		void ParticleSetSpawnedParticle(Particle& particle, Particle spawnedParticle, unsigned numberOfSpawnedParticles = 1);
		void ParticleSetCustomData(Particle& particle, void* data);

		void EmitterSetLocation(Emitter emitter, Vector2 location);
		void EmitterSetPoint(Emitter emitter, Vector2 location);
		void EmitterSetCircle(Emitter emitter, float radius, Vector2 location);
		void EmitterSetRectangle(Emitter emitter, Vector2 dimension, Vector2 location);
		void EmitterSetFrequency(Emitter emitter, float frequency, unsigned spawnCount = 1, bool spawnImmediately = false);
		void EmitterBurst(Emitter emitter, unsigned spawnedParticlesOverride = (unsigned)-1);
		void EmitterSetActive(Emitter emitter, bool state);

		ParticleDef* getDefinitionFromIndexParticles(unsigned& index);
		ParticleDef* getDefenitionFromIndexEmitters(unsigned& index);
		ParticleOutput* GetParticle(unsigned ParticleIndex);

	private:

		unsigned numDefinitions;
		ParticleDef* particleDefinitions;
		unsigned numParticles;

		unsigned numEmitters;
	};

	typedef ParticleSystem::ParticleOutput Output;

	/////////////////////////////////////////////////////////////////////
	// ParticleIterator
	/////////////////////////////////////////////////////////////////////

	class ParticleIterator
	{
	public:

		ParticleIterator(class ParticleSystem& particleSystem);
		ParticleIterator() = delete;

		void operator++();
		void operator++(int);
		const Output& operator*() const;
		const Output& operator->() const;
		operator bool() const;

	private:

		ParticleSystem* partSystem;

		Output* target;
		ParticleSystem::ParticleDef* currentDef;

		unsigned numDefinitions;
		unsigned numParticlesInDef;
		unsigned defIndex;
		unsigned particleIndex;

		bool reachedEnd;
	};

	/////////////////////////////////////////////////////////////////////
	// EmitterDebugOutput
	/////////////////////////////////////////////////////////////////////

	class EmitterDebugOutput
	{
	public:
		friend class ParticleSystem::ParticleDef;

		bool GetActive() { return active; };
		Vector2 GetLocation() { return location; };
		EmitterShape GetShape() { return shape; };
		Vector2 GetRectangleDimension() { return dims; };
		float GetCircleRadius() { return dims.X; };

	private:

		bool active;
		Vector2 location;
		Vector2 dims;
		EmitterShape shape;
	};

	/////////////////////////////////////////////////////////////////////
	// EmitterDebugIterator
	/////////////////////////////////////////////////////////////////////

	class EmitterDebugIterator
	{
	public:
		EmitterDebugIterator(class ParticleSystem& particleSystem);
		EmitterDebugIterator() = delete;

		unsigned CurrentIndex();

		void operator++();
		void operator++(int);
		const EmitterDebugOutput operator*() const;
		const EmitterDebugOutput operator->() const;
		operator bool() const;

	private:

		ParticleSystem* partSystem;

		EmitterDebugOutput target;
		ParticleSystem::ParticleDef* currentDef;

		unsigned numDefinitions;
		unsigned numEmittersInDef;
		unsigned defIndex;
		unsigned emitterIndex;

		unsigned indexCounter;

		bool reachedEnd;
	};
};