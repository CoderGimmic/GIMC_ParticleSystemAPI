// ParticleSystem.cpp

#include "ParticleSystem.h"

#include <cmath>

/*
	==================================
	Data Oriented Design Remake Plans
	==================================
	
	Creation of a particle and system
	---------------------------------
	int part = Particle::createParticle();
	Particle::setColor(part, c_red);

	int partSystem = Particle::createSystem();
	Particle::

	int emitter = Particle::createEmitter();

	ParticleComponent* comp = GameObject::addComponent();
	comp->assignEmitter(emitter);

	Particle Component structure
	----------------------------------
	ParticleComponent update
	{
		Particle::emitterSetPosition(m_emitter);
		Particle::emitterBurst();
	}

*/

namespace PS
{
	/////////////////////////////////////////////////////////////////////
	// Handles
	/////////////////////////////////////////////////////////////////////

	unsigned Particle::GetUniqueID()
	{
		return uniqueID;
	}

	unsigned Emitter::GetUniqueID()
	{
		return uniqueID;
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
		switch (shape)
		{
			case EmitterShape::CIRCLE:

			break;
			case EmitterShape::RECTANGLE:
				
			break;
		}
	}

	/////////////////////////////////////////////////////////////////////
	// ParticleData
	/////////////////////////////////////////////////////////////////////

	ParticleSystem::ParticleData::ParticleData()
	{
	}

	void ParticleSystem::ParticleData::Init(ParticleDef def, Vector2 newLocation)
	{
		location = newLocation;

		//TODO(Per): Initialize all values
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
		//TODO(Per): Convert from HSV to RGB and return it
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

	void ParticleSystem::ActivateEmitter(Emitter emitter)
	{

	}

	void ParticleSystem::DeactivateEmitter(Emitter emitter)
	{

	}

	bool ParticleSystem::DestroyParticle(Particle particle)
	{
		addFreeDefinitionSlot(particle.GetUniqueID());
	}

	bool ParticleSystem::DestroyEmitter(Emitter emitter)
	{
		addFreeEmitterSlot(emitter.GetUniqueID());
	}

	void ParticleSystem::ParticleSetLifetime(Particle particle, float minLife, float maxLife)
	{
		ParticleDef& def = particleDefenitions[particle.GetUniqueID()];
		def.minLife = minLife;
		def.maxLife = maxLife;
	}

	void ParticleSystem::ParticleSetColor(Particle particle, Color color)
	{
		ParticleSetColor(particle, color, color);
	}

	void ParticleSystem::ParticleSetColor(Particle particle, Color colorStart, Color colorEnd)
	{
		ParticleDef& def = particleDefenitions[particle.GetUniqueID()];
		def.colorStart = colorStart;
		def.colorEnd = colorEnd;
	}

	void ParticleSystem::ParticleSetVelocity(Particle particle, Vector2 velocity)
	{
		particleDefenitions[particle.GetUniqueID()].Velocity = velocity;
	}

	void ParticleSystem::ParticleSetCustomData(Particle particle, void* data)
	{
		particleDefenitions[particle.GetUniqueID()].customData = data;
	}

	void ParticleSystem::EmitterSetLocation(Emitter emitter, Vector2 location)
	{
		emitters[emitter.uniqueID].location = location;
	}

	void ParticleSystem::EmitterSetShape(Emitter emitter, EmitterShape shape)
	{
		emitters[emitter.GetUniqueID()].shape = shape;
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

	Vector2& Vector2::operator+(const Vector2& rhs)
	{
		return Vector2(X + rhs.X, Y + rhs.Y);
	}

	Vector2& Vector2::operator-(const Vector2& rhs)
	{
		return Vector2(X - rhs.X, Y - rhs.Y);
	}

	Vector2& Vector2::operator*(const Vector2& rhs)
	{
		return Vector2(X * rhs.X, Y * rhs.Y);
	}

	Vector2& Vector2::operator/(const Vector2& rhs)
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

	Color::Color(unsigned r, unsigned g, unsigned b, unsigned a)
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

	unsigned Color::clampValue(unsigned value)
	{
		if (value > 255)
			return 255;
		
		if (value < 0)
			return 0;
	}
};