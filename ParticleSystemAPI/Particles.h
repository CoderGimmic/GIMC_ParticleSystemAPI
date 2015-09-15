// Particles.h

#if 0

#pragma once

class ParticleSystem;

class Particles final
{
public:

	Particles(ParticleSystem* _particlesystem = nullptr, bool _active = true);

	void setSystem(ParticleSystem* _particlesystem);
	ParticleSystem* getSystem();
	void setActive(bool _state);
	void burst(float _deltatime);

private:

	ParticleSystem* m_particlesystem;
};

#endif