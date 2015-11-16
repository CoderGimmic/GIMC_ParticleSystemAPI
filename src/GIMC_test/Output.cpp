// Output.cpp

#include "Output.h"

#include "ParticleSystem.h"

namespace GIMC
{
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
};