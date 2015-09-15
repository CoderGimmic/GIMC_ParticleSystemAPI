// Particles.cpp

#if 0

#include "Particles.h"
#include "ParticleSystem.h"

Particles::Particles(ParticleSystem* _particlesystem, bool _active)
{
	m_particlesystem = _particlesystem;
	if (m_particlesystem)
	m_particlesystem->setActive(_active);
}

void Particles::setSystem(ParticleSystem* _emitter)
{
	m_particlesystem = _emitter;
}

ParticleSystem* Particles::getSystem()
{
	return m_particlesystem;
}

void Particles::setActive(bool _state)
{
	if (m_particlesystem == nullptr)
		return;

	m_particlesystem->setActive(_state);
}

void Particles::burst(float _deltatime)
{
	if (!m_particlesystem)
		return;

	m_particlesystem->burst(_deltatime);
}

#endif