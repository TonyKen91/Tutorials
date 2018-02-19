#include "Rigidbody.h"
#include "PhysicsScene.h"
#include <iostream>

#define MIN_LINEAR_THRESHOLD 0.1f
#define MIN_ROTATION_THRESHOLD 0.01f

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity, float linearDrag, float angularDrag) :
	PhysicsObject(shapeID), m_position (position), m_velocity (velocity), m_rotation(rotation), m_mass(mass), m_elasticity(elasticity), m_linearDrag(linearDrag), m_angularDrag(angularDrag)
{
	m_angularVelocity = 0;
}


Rigidbody::~Rigidbody()
{
}


void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	//if (m_isKinematic)
	//	return;

	m_velocity += gravity * timeStep;


	m_velocity *= (1 - m_linearDrag * timeStep);
	m_rotation += m_angularVelocity * timeStep;
	m_angularVelocity -= m_angularVelocity *m_angularDrag * timeStep;

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
		m_velocity = glm::vec2(0, 0);
	if (abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD)
		m_angularVelocity = 0;


	m_position += m_velocity * timeStep;
	applyForce(gravity * m_mass * timeStep, glm::vec2(0, 0));
}

void Rigidbody::debug()
{
}

void Rigidbody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	// These calculation calculates instantaneous velocity and angular velocity which are then added to the cumulative velocities
	m_velocity += force / m_mass;
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / m_inertia; // This adds torque/moment x and y component to the angular velocity per frame
}

//void Rigidbody::applyForceToActor(Rigidbody * actor2, glm::vec2 force)
//{
//	actor2->applyForce(force, glm::vec2(0, 0));
//	applyForce(-force, glm::vec2(0, 0));
//}

void Rigidbody::resolveCollision(Rigidbody * actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	// find the vector between their centres or use the provided direction of force
	glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(actor2->m_position - m_position);

	// get the vector perpendicular to the collision normal
	glm::vec2 perp(normal.y, -normal.x);

	// get the contact point when the collision occurs

	// determine the total velocity of the contact points for the two objects, for both linear and rotational

	// 'r' is the radius from axis to application of force
	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);

	// velocity of the contact point on this object
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;

	// velocity of contact point on actor2
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2) // they're moving closer
	{
		// calculate the effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_inertia);
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2 * r2) / actor2->m_inertia);

		float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		// apply equal and opposite forces
		applyForce(-force, contact - m_position);
		actor2->applyForce(force, contact - actor2->m_position);
	}

	// This part is the previous code before rotation tutorial

	//glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
	//glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

	//if (glm::dot(normal, relativeVelocity) >= 0)
	//	return;

	//float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;

	//float j = (glm::dot( -(1 + m_elasticity) * (relativeVelocity), normal) )/ glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->getMass())));

	//glm::vec2  force = normal * j;

	//applyForceToActor(actor2, force);
}

float Rigidbody::getPotentialGravitationalEnergy(glm::vec2 gravity)
{

	{return (-(m_mass)*glm::dot(gravity, m_position)); }
}
