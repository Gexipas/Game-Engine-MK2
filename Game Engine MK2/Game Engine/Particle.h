#pragma once

#include <glm/glm.hpp>

#include "camera.h"

class Particle
{
public:
	Particle(glm::vec3 _position, glm::vec3 _velocity, float _duration,	int _ID);
	~Particle() {}

	void Update(float _deltaTime);

	glm::vec3 m_position;
	glm::vec3 m_velocity;	
	float m_duration;
	float m_cameraDist;
	int m_ID;
private:
	
};

inline Particle::Particle(glm::vec3 _position, glm::vec3 _velocity, float _duration, int _ID)
{
	m_position = _position;
	m_velocity = _velocity;
	m_duration = _duration;
	m_cameraDist = glm::distance(Camera::instance().Position, m_position);
	m_ID = _ID;
}

inline void Particle::Update(float _deltaTime)
{
	m_cameraDist = glm::distance(Camera::instance().Position, m_position);
	m_velocity.y += -0.2f * _deltaTime;
	m_position += m_velocity;
	m_duration -= _deltaTime;
}