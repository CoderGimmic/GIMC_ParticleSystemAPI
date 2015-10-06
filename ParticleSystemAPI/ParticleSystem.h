// ParticleSystem.h

#pragma once

/*
	ParticleManager
		ParticleSystem
			Emitter
			Attractor
			Deflector
			Particle

*/

namespace PS
{
	class GenericHandle
	{
		friend class ParticleSystem;
	private:

		int uniqueID;
	};

	typedef GenericHandle Particle;
	typedef GenericHandle Emitter;

	enum class EmitterShape
	{
		RECTANGLE,
		CIRCLE,
	};

	class ParticleSystem
	{
		struct ParticleData
		{
		public:
			int typeIndex;
			Vector2 location;
			Color color;
			float lifeLeft;
		};

		class ParticleDef
		{
		public:
			ParticleDef();

		private:

			Vector2 Velocity;
			Color startColor, endColor;
			float minLife, maxLife;
		};

		class Emitterdef
		{
		public:
			Emitterdef();
			Emitterdef(EmitterShape newShape);

			int Update(float deltaTime);

			void SetLocation(Vector2 newLocation);
			Vector2 GetLocation();

		private:

			float frequency;
			Vector2 location;
			ParticleDef particle;
			EmitterShape shape;
			Vector2 size;
		};

	public:

		ParticleSystem();

		void Update(float deltaTime);

		Particle CreateParticle();
		Emitter CreateEmitter();
		void ActivateEmitter(Emitter emitter);
		void DeactivateEmitter(Emitter emitter);
		bool DestroyParticle(Particle particle);
		bool DestroyEmitter(Emitter emitter);

		void ParticleSetColor(Particle particle, Color color);
		void ParticleSetColor(Particle particle, Color startColor, Color endColor);
		void ParticleSetVelocity(Particle particle, Vector2 valocity);

		void EmitterSetShape(Emitter emitter, EmitterShape shape);
		void EmitterSetSize(Emitter emitter, Vector2 size);

		unsigned GetParticleCount();
		Vector2 GetParticleLocation(unsigned particleIndex);
		Color GetParticleColor(unsigned particleIndex);

	private:

		int getUniqueID();

	private:

		static const unsigned int MAX_PARTICLE_DEFS = 1000;
		ParticleDef particleDefenitions[MAX_PARTICLE_DEFS];

		static const unsigned int MAX_PARTICLES = 10000;
		unsigned numActiveParticles;
		ParticleData activeParticles[MAX_PARTICLES];

		static const unsigned int MAX_EMITTERS = 1000;
		unsigned numActiveEmitters;
		Emitterdef activeEmitters[MAX_EMITTERS];

		unsigned int uniqueID;
	};

	struct Vector2
	{
		Vector2();
		Vector2(int x, int y);

		Vector2& operator=(const Vector2& rhs);
		Vector2& operator+=(const Vector2& rhs);
		Vector2& operator-=(const Vector2& rhs);

		Vector2& operator*=(const float value);
		Vector2& operator/=(const float value);

		Vector2& operator+(const Vector2& rhs);
		Vector2& operator-(const Vector2& rhs);
		Vector2& operator*(const Vector2& rhs);
		Vector2& operator/(const Vector2& rhs);

		float length();
		void normalize();

		float X, Y;
	};

	struct Color
	{
		Color();
		Color(float r, float g, float b);

		float R, G, B;

	private:

		void clampColor();
		float clampValue(float value);
	};
};