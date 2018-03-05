#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include <iostream>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour,
	float rotation, float elasticity, float linearDrag, float angularDrag) :
	Rigidbody(SPHERE, position, velocity, mass, rotation, elasticity, linearDrag, angularDrag)
{
	m_radius = radius;
	m_colour = colour;
	m_inertia = 0.5f * m_mass * m_radius * m_radius;
	m_originalInertia = m_inertia;
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

void Sphere::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::fixedUpdate(gravity, timeStep);
	gridCollision = 0;
	inGrid = false;

	// store the local axes
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void Sphere::CollideWithSphere(Sphere * pOther)
{
	glm::vec2 delta = pOther->getPosition() - m_position;
	float distance = glm::length(delta);
	float intersection = m_radius + pOther->getRadius() - distance;

	if (intersection > 0)
	{

		glm::vec2 contactForce = 0.5f * (distance - (m_radius + pOther->getRadius()))*delta / distance;
		//m_position += contactForce;
		//pOther->m_position -= contactForce;

		// move each shape away in the direction of penetration
		if (!m_isKinematic && !pOther->isKinematic())
		{
			m_position += contactForce;
			pOther->m_position -= contactForce;
		}
		else if (!m_isKinematic)
			m_position += contactForce * 2.0f;
		else if (!pOther->isKinematic())
			pOther->m_position -= contactForce * 2.0f;

		if (pOther->m_name == "Grid Ball")
		{
			gridCollision++;
			if (gridCollision >= 4)
				inGrid = true;
		}

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

//
//float Sphere::getLinearKineticEnergy()
//{
//	float linearKinetic = ((0.5f)*m_mass* glm::dot(m_velocity, m_velocity));
//	std::cout << "Velocity" << m_velocity.y << std::endl;
//	std::cout << linearKinetic << std::endl;
//	return linearKinetic;
//}
