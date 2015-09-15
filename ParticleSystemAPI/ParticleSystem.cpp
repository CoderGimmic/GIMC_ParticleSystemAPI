// ParticleSystem.cpp

#if 0

#include "ParticleSystem.h"

#include "Time.h"
#include "DrawManager.h"

#include "SFML\Graphics.hpp"
#include "AnimatedSprite.h"

#include <algorithm>
#include "Random.h"
#include "Math.h"

#include <iostream>

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

Emitter::Emitter(TYPE _type, float _x, float _y, float _width, float _height)
{
	m_type = _type;
	m_x = _x; m_y = _y;
	m_width = _width; m_height = _height;

	m_particle = nullptr;

	m_timer = 0.0f;
	m_frequency = 0.0f;
}

void Emitter::setPosition(sf::Vector2f _pos)
{
	m_x = _pos.x;
	m_y = _pos.y;
}

sf::Vector2f Emitter::getPosition()
{
	return sf::Vector2f(m_x,m_y);
}

void Emitter::setOwner(ParticleSystem* _system)
{
	m_owner = _system;
}

void Emitter::setSize(float _width, float _height)
{
	m_width = _width;
	m_height = _height;
}

void Emitter::setType(TYPE _type)
{
	m_type = _type;
}

void Emitter::setParticle(Particle* _particle, float _frequency)
{
	m_particle = _particle;
	m_frequency = _frequency;
}

Particle* Emitter::getParticle()
{
	if (m_particle)
	return m_particle;
	else
	return nullptr;
}

void Emitter::setFrequency(float _frequency)
{
	m_frequency = _frequency;
}

float Emitter::getFrequency()
{
	return m_frequency;
}

sf::Vector2f Emitter::getStart()
{
	switch(m_type)
	{
	case POINT:
			return sf::Vector2f(m_x,m_y);
		break;
	case RECT:
			return sf::Vector2f(m_x + Random::betweenf(.0f,m_width),m_y + Random::betweenf(.0f,m_height));
		break;
	case CIRCLE:
			float angle = Random::betweenf(0.f,Math::PI*2.f);
			float radius = Random::betweenf(0.0f, (m_width + m_height) / 2.f);

			float x = m_x + radius * cosf(angle);
			float y = m_y + radius * sinf(angle);

			return sf::Vector2f(x,y);
		break;
	}

	return sf::Vector2f(0,0);
}

int Emitter::getCount(float _deltatime)
{
	if (_deltatime == 0.0f)
		return 0;

	if ( !m_owner->getBurst())
	{
		int count = (int)floorf((_deltatime + m_timer) / (1.f /m_frequency));

		m_timer = fmod(_deltatime + m_timer, 1.f / m_frequency);

		return count;
	}
	else
		return (int)m_frequency;
}

Particle::Particle(TYPE _type,float _life_min,float _life_max)
{
	m_type = _type;
	m_life = m_life_min;

	m_life_min = _life_min;
	m_life_max = _life_max;

	m_color_h = 0.0f;
	m_color_s = 0.0f;
	m_color_l = 0.0f;
	m_color_a = 0.0f;

	m_color_dif_h = 0.0f;
	m_color_dif_s = 0.0f;
	m_color_dif_l = 0.0f;
	m_color_dif_a = 0.0f;

	m_scale_x = 1.f;
	m_scale_y = 1.f;

	m_size_const = true;
	m_size = 0.0f;
	m_size_min = 0.0f;
	m_size_max = 0.0f;
	m_size_inc = 0.0f;
	m_size_wiggle = 0.0f;

	m_rot_relative = false;
	m_rot_const = true;
	m_rot = 0.0f;
	m_rot_min = 0.0f;
	m_rot_max = 0.0f;
	m_rot_inc = 0.0f;
	m_rot_wiggle = 0.0f;

	m_dir_const = true;
	m_dir = 0.0f;
	m_dir_min = 0.0f;
	m_dir_max = 0.0f;
	m_dir_inc = 0.0f;
	m_dir_wiggle = 0.0f;

	m_speed_const = true;
	m_speed = 0.0f;
	m_speed_min = 0.0f;
	m_speed_max = 0.0f;
	m_speed_inc = 0.0f;
	m_speed_wiggle = 0.0f;

	m_x = 0.0f;
	m_y = 0.0f;
	
	m_vel_x = 0.f;
	m_vel_y = 0.f;

	m_sprite = nullptr;
	m_particle = nullptr;
}

Particle::~Particle()
{

}

Particle* Particle::instantiate(sf::Vector2f _pos, int _depth)
{
	Particle* particle = new Particle(m_type, m_life_min,m_life_max);

	particle->m_depth = _depth;

	//if (m_life_min != m_life_max)
	particle->m_life = Random::betweenf(m_life_min,m_life_max);
	//else particle->m_life = m_life_min;

	//if (m_dir_min != m_dir_max)
	particle->m_dir = Random::betweenf(m_dir_min,m_dir_max);
	//else particle->m_dir = m_dir_min;

	//if (m_speed_min != m_speed_max)
	particle->m_speed = Random::betweenf(m_speed_min,m_speed_max);
	//else particle->m_speed = m_speed_min;

	//if (m_size_min != m_size_max)
	particle->m_size = Random::betweenf(m_size_min,m_size_max);
	//else particle->m_size = m_size_min;

	//if (m_rot_min != m_rot_max)
	particle->m_rot = Random::betweenf(m_rot_min,m_rot_max);
	//else particle->m_rot = m_rot_min;

	particle->setSize(m_size_min,m_size_max,m_size_inc,m_size_wiggle);
	particle->setRotation(m_rot_min, m_rot_max, m_rot_inc, m_rot_wiggle, m_rot_relative);
	particle->setDirection(m_dir_min, m_dir_max, m_dir_inc, m_dir_wiggle);
	particle->setSpeed(m_speed_min, m_speed_max, m_speed_inc, m_speed_wiggle);
	particle->setParticle(m_particle);
	
	particle->setColor(m_color_start,m_color_end,m_blend);
	particle->setSprite(m_sprite);
	particle->setPosition(_pos);
	particle->setScale(m_scale_x,m_scale_y);
	particle->setSprite(m_sprite);

	if (particle->m_size_inc != 0.0f || particle->m_size_wiggle != 0.0f)
		particle->m_size_const = false;

	if ((particle->m_rot_inc != 0.0f || particle->m_rot_wiggle != 0.0f) && !m_rot_relative)
		particle->m_rot_const = false;

	if (particle->m_dir_inc != 0.0f || particle->m_dir_wiggle != 0.0f)
		particle->m_dir_const = false;

	if (particle->m_speed_inc != 0.0f || particle->m_speed_wiggle != 0.0f)
		particle->m_speed_const = false;

	return particle;
}

void Particle::setSize(float _size_min, float _size_max, float _size_inc, float _size_wiggle)
{
	m_size_min = std::min(_size_min,_size_max);
	m_size_max = std::max(_size_min,_size_max);
	m_size_inc = _size_inc;
	m_size_wiggle = _size_wiggle;
}

void Particle::setScale(float _scale_x, float _scale_y)
{
	m_scale_x = _scale_x;
	m_scale_y = _scale_y;
}

void Particle::setColor(sf::Color _start, sf::Color _end, sf::BlendMode _blend)
{
	m_color_start = _start;
	m_color_end = _end;
	m_blend = _blend;

	HSL color_start = TurnToHSL(m_color_start);

	m_color_h = (float)color_start.Hue;
	m_color_s = (float)color_start.Saturation;
	m_color_l = (float)color_start.Luminance;
	m_color_a = (float)m_color_start.a;

	if ( m_color_start == m_color_end)
		return;

	HSL color_end = TurnToHSL(m_color_end);

	if (color_start.Hue != color_end.Hue)
		m_color_dif_h = float(color_start.Hue - color_end.Hue)*-1;

	if (color_start.Saturation != color_end.Saturation)
		m_color_dif_s = float(color_start.Saturation - color_end.Saturation)*-1;

	if (color_start.Luminance != color_end.Luminance)
		m_color_dif_l = float(color_start.Luminance - color_end.Luminance)*-1;

	if (m_color_start.a != m_color_end.a)
		m_color_dif_a = float(m_color_start.a - m_color_end.a)*-1 ;

	m_color_dif_h /= m_life;
	m_color_dif_s /= m_life;
	m_color_dif_l /= m_life;
	m_color_dif_a /= m_life;
}

void Particle::setSprite(AnimatedSprite* _sprite)
{
	if (m_type != SPRITE)
		return;
	else
	m_sprite = _sprite;
}

void Particle::setSpeed(float _speed_min, float _speed_max, float _speed_inc, float _speed_wiggle)
{
	m_speed_min = std::min(_speed_min, _speed_max);
	m_speed_max = std::max(_speed_min, _speed_max);
	m_speed_inc = _speed_inc;
	m_speed_wiggle = _speed_wiggle;
}

void Particle::setDirection(float _dir_min, float _dir_max, float _dir_inc, float _dir_wiggle)
{
	m_dir_min = std::min(_dir_min, _dir_max);
	m_dir_max = std::max(_dir_min, _dir_max);
	m_dir_inc = _dir_inc;
	m_dir_wiggle = _dir_wiggle;
}

void Particle::setRotation(float _rot_min, float _rot_max, float _rot_inc, float _rot_wiggle, bool _rot_relative)
{
	m_rot_min = std::min(_rot_min, _rot_max);
	m_rot_max = std::max(_rot_min, _rot_max);
	m_rot_inc = _rot_inc;
	m_rot_wiggle = _rot_wiggle;
	m_rot_relative = _rot_relative;
}

void Particle::setPosition(sf::Vector2f _pos)
{
	m_x = _pos.x;
	m_y = _pos.y;
}

sf::Vector2f Particle::getPosition()
{
	return sf::Vector2f(m_x, m_y);
}

void Particle::setParticle(Particle* _particle)
{
	m_particle = _particle;
}

Particle* Particle::getParticle()
{
	if (m_particle)
	return m_particle;
	else
	return nullptr;
}

bool Particle::isDead()
{
	return (m_life < 0.0f);
}

void Particle::update(float _deltatime)
{
	if (_deltatime == 0.0f)
		return;

	// COLORS
	m_color_h += m_color_dif_h*_deltatime;
	m_color_s += m_color_dif_s*_deltatime;
	m_color_l += m_color_dif_l*_deltatime;
	m_color_a += m_color_dif_a*_deltatime;
	
	// HUE
	if (m_color_h > 360)
		m_color_h = 360;
	if (m_color_h < 0)
		m_color_h = 0;

	// SATURATION
	if (m_color_s > 100)
		m_color_s = 100;
	if (m_color_s < 0)
		m_color_s = 0;

	// LUMINESCENCE
	if (m_color_l > 100)
		m_color_l = 100;
	if (m_color_l < 0)
		m_color_l = 0;

	// ALPHA
	if (m_color_a > 255)
		m_color_a = 255;
	if (m_color_a < 0)
		m_color_a = 0;
		
	// TRANSFORMATIONS
	if ( !m_size_const)
		m_size += Random::betweenf(m_size_wiggle*-1,m_size_wiggle) + m_size_inc*_deltatime;

	if ( !m_rot_const && !m_rot_relative)
		m_rot += Random::betweenf(m_rot_wiggle*-1,m_rot_wiggle) + m_rot_inc*_deltatime;
	
	if ( !m_dir_const)
		m_dir += Random::betweenf(m_dir_wiggle*-1,m_dir_wiggle) + m_dir_inc*_deltatime;

	if ( !m_speed_const)
		m_speed += Random::betweenf(m_speed_wiggle*-1,m_speed_wiggle) + m_speed_inc*_deltatime;

	if (m_rot_relative)
		m_rot = m_dir + Random::betweenf(m_rot_wiggle*-1,m_rot_wiggle);

	// VELOCITY
	if ( !m_speed_const || !m_dir_const || (m_vel_x == 0.0f && m_vel_y == 0.0f))
	{
		float angle = m_dir * Math::degToRad;
		sf::Vector2f dir( cos(angle), sin(angle) );

		float hyp = sqrtf( dir.x*dir.x + dir.y*dir.y);
		if (hyp > 0.0f)
		{
			dir.x /= hyp;
			dir.y /= hyp;
		}

		m_vel_x = dir.x*m_speed;
		m_vel_y = dir.y*m_speed;
	}

	m_life -= _deltatime;

	m_x += m_vel_x*_deltatime;
	m_y += m_vel_y*_deltatime;
}

void Particle::draw(DrawManager* _window)
{
	sf::Color color = HSL((int)m_color_h,(int)m_color_s,(int)m_color_l).TurnToRGB();
	color.a = (sf::Uint8)m_color_a;
	switch(m_type)
	{
		case PIXEL:
		{
				sf::RectangleShape shape(sf::Vector2f(1,1));
				shape.setFillColor(color);
				shape.setPosition(m_x, m_y);
				_window->draw(shape, m_blend, m_depth);
			break;
		}
		case CIRCLE:
		{
				float size = m_size*((m_scale_x + m_scale_y)/2);
				sf::CircleShape shape(size);
				shape.setOrigin(size / 2, size / 2);
				shape.setFillColor(color);
				shape.setPosition(m_x, m_y);
				_window->draw(shape, m_blend, m_depth);
			break;
		}
		case RECT:
		{
				float x_size = m_size * m_scale_x;
				float y_size = m_size * m_scale_y;
				sf::RectangleShape shape(sf::Vector2f(x_size , y_size));
				shape.setOrigin(x_size / 2 , y_size / 2);
				shape.setFillColor(color);
				shape.setRotation(m_rot);
				shape.setPosition(m_x, m_y);
				_window->draw(shape, m_blend, m_depth);
			break;
		}
		case SPRITE:
		{
				if (!m_sprite)
					return;

				m_sprite->setColor(color);
				m_sprite->setScale((m_size / m_sprite->getLocalBounds().width) * m_scale_x, (m_size / m_sprite->getLocalBounds().height) * m_scale_y);
				m_sprite->setRotation(m_rot);
				m_sprite->setPosition(m_x, m_y);
				m_sprite->setOpacity((int)m_color_a);
				_window->draw(*m_sprite, m_blend, m_depth);
			break;
		}
	}
}

ParticleSystem::ParticleSystem(bool _burst)
{
	m_active = true;
	m_burst = _burst;
	m_depth = 0;
}

ParticleSystem::~ParticleSystem()
{
	for(auto& e: m_emitters)
	{
		delete e.second;
		e.second = nullptr;
	}
	m_emitters.clear();

	for(auto& p: m_particles)
	{
		delete p;
		p = nullptr;
	}
	m_particles.clear();
}

void ParticleSystem::addEmitter(std::string _name, Emitter* _emitter)
{
	_emitter->setOwner(this);
	m_emitters.insert(std::pair<std::string,Emitter*>(_name,_emitter));
}

Emitter* ParticleSystem::getEmitter(std::string _name)
{
	std::map<std::string, Emitter*>::iterator it = m_emitters.find(_name);

	if (it != m_emitters.end())
	{
		return it->second;
	}
	return nullptr;
}

int ParticleSystem::ParticleCount()
{
	return m_particles.size();
}

void ParticleSystem::setParticlePosition(sf::Vector2f _pos)
{
	for(auto& p: m_particles)
	{
		p->setPosition(_pos);
	}
}

void ParticleSystem::setDepth(int _value)
{
	m_depth = _value;
}

void ParticleSystem::setActive(bool _active)
{
	m_active = _active;
}

bool ParticleSystem::getActive()
{
	return m_active;
}

void ParticleSystem::setBurst(bool _burst)
{
	m_burst = _burst;
}

bool ParticleSystem::getBurst()
{
	return m_burst;
}

void ParticleSystem::clear()
{
	for(auto& p: m_particles)
	{
		delete p;
		p = nullptr;
	}

	m_particles.clear();
}

void ParticleSystem::addParticle(Particle* _part)
{
	m_particles.push_back(_part);
}

void ParticleSystem::createParticles(float _deltatime)
{
	for(auto& e: m_emitters)
	{
		int count = e.second->getCount(_deltatime);
		for(int i = 0; i < count; i++)
		{
			Particle* blueprint = e.second->getParticle();
			if (blueprint)
				addParticle(blueprint->instantiate(e.second->getStart(),m_depth));
		}
	}
}

void ParticleSystem::burst(float _deltatime)
{
	if ( !m_active)
		return;

	createParticles(_deltatime);
}

void ParticleSystem::update(float _deltatime)
{
	if ( !m_active)
		return;

	if (!m_burst)
		createParticles(_deltatime);

	std::vector<Particle*> new_particles;

	std::vector<Particle*>::iterator it;
	for (it = m_particles.begin(); it != m_particles.end(); )
	{
		(*it)->update(_deltatime);
		if ((*it)->isDead())
		{
			Particle* blueprint = (*it)->getParticle();
			if (blueprint)
			{
				Particle* p = blueprint->instantiate((*it)->getPosition(),m_depth);
				new_particles.push_back(p);
			}

			delete (*it);
			(*it) = nullptr;

			it = m_particles.erase(it);
		}
		else
			it++;
	}

	for(auto& part: new_particles)
		m_particles.push_back(part);
}

void ParticleSystem::draw(DrawManager* _window)
{
	if (!m_active)
		return;

	for(auto& p: m_particles)
		p->draw(_window);
}

#endif