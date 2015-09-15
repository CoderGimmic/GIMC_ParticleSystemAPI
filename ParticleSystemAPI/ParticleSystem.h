// ParticleSystem.h

#if 0

#pragma once

#include <vector>
#include <map>

#include "SFML\Graphics\BlendMode.hpp"
#include "SFML\Graphics\Color.hpp"
#include "SFML\System\Vector2.hpp"

#include "HSL.h"

/*
	ParticleManager
		ParticleSystem
			Emitter
			Attractor
			Deflector
			Particle

*/

namespace sf
{
	class RenderWindow;
	class Sprite;
}

class Particle;
class AnimatedSprite;
class ParticleSystem;

class DrawManager;

class Emitter
{
public:
	enum TYPE
	{
		POINT,
		RECT,
		CIRCLE,
		COUNT
	};
	Emitter(TYPE _type, float _x, float _y, float _width, float _height);
	void setPosition(sf::Vector2f _pos);
	sf::Vector2f getPosition();

	void setOwner(ParticleSystem* _system);
	void setSize(float _width, float _height);
	void setType(TYPE _type);
	
	void setParticle(Particle* _particle, float _frequency);
	Particle* getParticle();

	void setFrequency(float _frequency);
	float getFrequency();

	sf::Vector2f getStart();
	
	int getCount(float _deltatime);

private:

	TYPE m_type;
	Particle* m_particle;

	ParticleSystem* m_owner;

	float m_x;
	float m_y;
	float m_width;
	float m_height;

	float m_timer;
	float m_frequency;
};

class Particle
{
public:
	enum TYPE
	{
		PIXEL,
		CIRCLE,
		RECT,
		SPRITE
	};

	Particle(TYPE _type,float _life_min,float _life_max);
	~Particle();

	Particle* instantiate(sf::Vector2f _pos, int _depth);

	void setSize(float _size_min, float _size_max, float _size_inc = 0.0f, float _size_wiggle = 0.0f);
	void setScale(float _scale_x,float _scale_y);

	void setColor(sf::Color _start, sf::Color _end, sf::BlendMode = sf::BlendAlpha);
	void setSprite(AnimatedSprite* _sprite);
	
	void setSpeed(float _speed_min, float _speed_max, float _speed_inc = 0.0f, float _speed_wiggle = 0.0f);
	void setDirection(float _dir_min, float _dir_max, float _dir_inc = 0.0f, float _dir_wiggle = 0.0f);
	void setRotation(float _rot_min, float _rot_max, float _rot_inc = 0.0f, float _rot_wiggle = 0.0f, bool _rot_relative = false);
	void setPosition(sf::Vector2f _pos);
	sf::Vector2f getPosition();

	void setParticle(Particle* _particle);
	Particle* getParticle();

	bool isDead();

	void update(float _deltatime);
	void draw(DrawManager* _window);

private:

	TYPE m_type;

	float m_x;
	float m_y;

	AnimatedSprite* m_sprite;
	Particle* m_particle;

	int m_depth;
	
	// Life
	float m_life;
	float m_life_min; float m_life_max;

	// Scale
	float m_scale_x; float m_scale_y;

	// Size
	bool m_size_const;
	float m_size;
	float m_size_min; float m_size_max;
	float m_size_inc;
	float m_size_wiggle;

	// Rotation
	bool m_rot_relative;
	bool m_rot_const;
	float m_rot;
	float m_rot_min; float m_rot_max;
	float m_rot_inc;
	float m_rot_wiggle;

	// Direction
	bool m_dir_const;
	float m_dir;
	float m_dir_min; float m_dir_max;
	float m_dir_inc; float m_dir_wiggle;

	// Speed
	bool m_speed_const;
	float m_speed;
	float m_speed_min; float m_speed_max;
	float m_speed_inc; float m_speed_wiggle;

	// Velocity
	float m_vel_x; float m_vel_y;

	// Color
	float m_color_h, m_color_s, m_color_l, m_color_a;
	float m_color_dif_h, m_color_dif_s, m_color_dif_l, m_color_dif_a;
	sf::BlendMode m_blend;

	sf::Color m_color_start;
	sf::Color m_color_end;
};

class ParticleSystem
{
public:
	ParticleSystem(bool _burst = false);
	~ParticleSystem();

	void addEmitter(std::string _name, Emitter* _emitter);
	Emitter* getEmitter(std::string _name);

	void addParticle(Particle* _particle);
	
	int ParticleCount();
	void setParticlePosition(sf::Vector2f _pos);

	void setDepth(int _value);

	void setActive(bool _active);
	bool getActive();
	void setBurst(bool _burst);
	bool getBurst();

	void createParticles(float _deltatime);

	void clear();

	void burst(float _deltatime);

	void update(float _deltatime);
	void draw(DrawManager* _window);

private:
	
private:
	std::map<std::string,Emitter*> m_emitters;
	std::vector<Particle*> m_particles;

	int m_depth;
	bool m_burst;
	bool m_active;
};

#endif