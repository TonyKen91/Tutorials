#include "Sphere.h"
#include <iostream>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) :
	Rigidbody(SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_colour = colour;
}


Sphere::~Sphere()
{
}

void Sphere::draw()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 10, m_colour);
}

bool Sphere::checkCollision(PhysicsObject * pOther)
{
	Sphere* pOtherSphere = dynamic_cast<Sphere*>(pOther);
	if (pOtherSphere != nullptr)
	{
		if ((m_radius + pOtherSphere->m_radius) > glm::distance(m_position, pOtherSphere->m_position))
			return true;
		else
			return false;
	}
	return false;
}
