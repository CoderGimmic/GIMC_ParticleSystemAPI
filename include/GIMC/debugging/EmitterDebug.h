// EmitterDebug.h

#pragma once

#include "..\Vector2.h"
#include "..\Emitter.h"

namespace GIMC
{
	/////////////////////////////////////////////////////////////////////
	// EmitterDebugOutput
	/////////////////////////////////////////////////////////////////////

	namespace internal
	{
		class ParticleDef;
	}

	class EmitterDebugOutput
	{
		friend class internal::ParticleDef;

	public:

		bool GetActive() { return active; };
		Vector2 GetLocation() { return location; };
		EmitterShape GetShape() { return shape; };
		Vector2 GetRectangleDimension() { return dims; };
		float GetCircleRadius() { return dims.X; };
		float GetRim() { return rim; };

	private:

		bool active;
		Vector2 location;
		EmitterShape shape;
		Vector2 dims;
		float rim;
	};

	/////////////////////////////////////////////////////////////////////
	// EmitterDebugIterator
	/////////////////////////////////////////////////////////////////////

	namespace internal
	{
		class ParticleDef;
	};
	
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
		internal::ParticleDef* currentDef;

		unsigned numDefinitions;
		unsigned numEmittersInDef;
		unsigned defIndex;
		unsigned emitterIndex;

		unsigned indexCounter;

		bool reachedEnd;
	};
};