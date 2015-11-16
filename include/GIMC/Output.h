// Output.h

#pragma once

#include "Vector2.h"
#include "Color.h"
#include "internal\HSL.h"

namespace GIMC
{
	namespace internal
	{
		class ParticleDef;
	};

	struct ParticleOutput
	{
		friend class ParticleSystem;
		friend class internal::ParticleDef;

		ParticleOutput()
			: scale(1.0f, 1.0f)
			, rotation(0.0f)
			, size(1.0f)
			, locationData()
			, colorData()
		{}

		~ParticleOutput()
		{}

	public:

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

			void Set(internal::HSL hsl, float a)
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

	private:

		float lifeRemaining;

		LocationData locationData;
		ColorData colorData;
	};

	typedef ParticleOutput Output;

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
		internal::ParticleDef* currentDef;

		unsigned numDefinitions;
		unsigned numParticlesInDef;
		unsigned defIndex;
		unsigned particleIndex;

		bool reachedEnd;
	};
};