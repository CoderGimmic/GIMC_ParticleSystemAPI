// ParticleSystem.cpp

#include "ParticleSystem.h"

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