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
	// ParticleDef
	/////////////////////////////////////////////////////////////////////

	ParticleSystem::ParticleDef::ParticleDef()
	{

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

	void ParticleSystem::Emitterdef::SetLocation(Vector2 newLocation)
	{
		location = newLocation;
	}

	Vector2 ParticleSystem::Emitterdef::GetLocation()
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
	// ParticleSystem
	/////////////////////////////////////////////////////////////////////

	ParticleSystem::ParticleSystem()
	{
		uniqueID = 0;
	}

	void ParticleSystem::Update(float deltaTime)
	{
		for (unsigned i = 0; i < numActiveEmitters; i++)
		{
			unsigned newParticles = activeEmitters[i].Update(deltaTime);

			for (unsigned j = 0; j < newParticles; j++)
			{

			}
		}
	}

	Particle ParticleSystem::CreateParticle()
	{
		Particle handle;
		handle.uniqueID = getUniqueID();
		return handle;
	}

	Emitter ParticleSystem::CreateEmitter()
	{
		Emitter handle;
		handle.uniqueID = getUniqueID();
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

	}

	bool ParticleSystem::DestroyEmitter(Emitter emitter)
	{

	}

	void ParticleSystem::ParticleSetColor(Particle particle, Color color)
	{
		ParticleSetColor(particle, color, color);
	}

	void ParticleSystem::ParticleSetColor(Particle particle, Color startColor, Color endColor)
	{

	}

	void ParticleSystem::EmitterSetShape(Emitter emitter, EmitterShape shape)
	{
		
	}

	unsigned ParticleSystem::GetParticleCount()
	{
		return numActiveParticles;
	}

	Vector2 ParticleSystem::GetParticleLocation(unsigned particleIndex)
	{
		if (particleIndex > numActiveParticles)
			return Vector2();

		return activeParticles[particleIndex].location;
	}

	Color ParticleSystem::GetParticleColor(unsigned particleIndex)
	{
		if (particleIndex > numActiveParticles)
			return Color();

		return activeParticles[particleIndex].color;
	}

	// Private

	int ParticleSystem::getUniqueID()
	{
		return uniqueID++;
	}

	/////////////////////////////////////////////////////////////////////
	// Vector2
	/////////////////////////////////////////////////////////////////////

	Vector2::Vector2()
		: X(0)
		, Y(0)
	{
	}

	Vector2::Vector2(int x, int y)
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
		: R(1.0f)
		, G(1.0f)
		, B(1.0f)
	{
	}

	Color::Color(float r, float g, float b)
		: R(r)
		, G(g)
		, B(b)
	{
		clampColor();
	}

	// Private

	void Color::clampColor()
	{
		R = clampValue(R);
		G = clampValue(G);
		B = clampValue(B);
	}

	float Color::clampValue(float value)
	{
		if (value > 1.0f)
			return 1.0f;
		
		if (value < 0.0f)
			return 0.0f;
	}
};