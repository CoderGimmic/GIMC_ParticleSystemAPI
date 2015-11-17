// EmitterDebug.cpp

#include "debugging/EmitterDebug.h"

#include "ParticleSystem.h"

namespace GIMC
{
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