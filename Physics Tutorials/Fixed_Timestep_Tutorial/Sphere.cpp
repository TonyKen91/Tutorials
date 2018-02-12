#include "Sphere.h"
#include "Plane.h"
#include <iostream>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) :
	Rigidbody(SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_colour = colour;
	m_inertia = 0.5f * m_mass * m_radius * m_radius;
}


Sphere::~Sphere()
{
}

void Sphere::draw()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, m_segments, m_colour);
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

void Sphere::CollideWithSphere(Sphere * pOther)
{
	if ((m_radius + pOther->m_radius) > glm::distance(m_position, pOther->m_position))
	{
		resolveCollision(pOther, , );
	}

}

void Sphere::CollideWithPlane(Plane* pOther)
{
	glm::vec2 collisionNormal = pOther->getNormal();

	float sphereToPlane = glm::dot(m_position, pOther->getNormal()) - pOther->getDistance();

	// if the sphere is behind the plane then both need to be flipped
	if (sphereToPlane < 0)
	{
		collisionNormal *= -1;
		sphereToPlane *= -1;
	}

	float interesection = m_radius - sphereToPlane;
	if (interesection > 0)
	{
		pOther->resolveCollision(this);
	}
}

void Sphere::CollideWithBox(Box * pOther)
{
}
