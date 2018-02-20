#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include <iostream>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) :
	Rigidbody(SPHERE, position, velocity, 0, mass, 1, 0, 0)
{
	m_radius = radius;
	m_colour = colour;
	m_inertia = 0.5f * m_mass * m_radius * m_radius;
	m_angularVelocity;
}


Sphere::~Sphere()
{
}

void Sphere::draw()
{
	glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation))* m_radius;

	aie::Gizmos::add2DCircle(m_position, m_radius, m_segments, m_colour);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1, 1, 1, 1));
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
	glm::vec2 delta = pOther->getPosition() - m_position;
	float distance = glm::length(delta);
	float intersection = m_radius + pOther->getRadius() - distance;

	if (intersection > 0)
	{
		glm::vec2 contactForce = 0.5f * (distance - (m_radius + pOther->getRadius()))*delta / distance;
		m_position += contactForce;
		pOther->m_position -= contactForce;

		// respond to the collision
		resolveCollision(pOther, 0.5f * (m_position + pOther->getPosition()));
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

	float intersection = m_radius - sphereToPlane;
	if (intersection > 0)
	{
		glm::vec2 contact = m_position + (collisionNormal * -m_radius);
		pOther->resolveCollision(this, contact);

		// contact force
		m_position += collisionNormal * (m_radius - sphereToPlane);
	}
}

void Sphere::CollideWithBox(Box * pOther)
{
	pOther->CollideWithSphere(this);
}
