#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////////////////
// This is sphere's constructor which creates the instance of sphere
// It takes in the following arguments:
// glm::vec2 position which is the location of the sphere in reference to the Gizmos' origin
// glm::vec2 velocity which determines the sphere's initial velocity
// float mass which is the mass of the sphere which is used in physics calculation
// float radius which determines the size of the sphere
// glm::vec4 colour uses a RGBA to determine the colour and opacity of the object
// float rotation which states the initial orientation
// float elasticity defines how elastic it is which determine its bounce
// float linearDrag which is used to reduce the linear velocity
// float angularDrag which is used to dampen the angular velocity of the sphere
/////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to draw the sphere on the scene
// This also creates a line from the centre to the edge of the circle to show orientation of the circle
////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sphere::draw()
{
	// This is the position of the end of the line
	glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation))* m_radius;

	aie::Gizmos::add2DCircle(m_position, m_radius, m_segments, m_colour);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1, 1, 1, 1));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function handles the physics calculation for sphere and updates its properties
// This also calculate the local axes of the object
// It takes in the following arguments:
// glm::vec2 gravity which is the acceleration due to gravity
// float timeStep which is the timeStep that defines the frame rate of the scene
////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to check sphere to sphere collision
// It then calls resolveCollision function if a collision is detected
// It takes in the following arguments:
// Sphere* pOther is a pointer reference to the second sphere it's checking collision with
////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sphere::CollideWithSphere(Sphere * pOther)
{
	// calculates the displacement of the other sphere in relation to this sphere's position
	glm::vec2 delta = pOther->getPosition() - m_position;
	float distance = glm::length(delta);

	// Checks if collision occurred
	float intersection = m_radius + pOther->getRadius() - distance;

	// If a collision occured this part check if it has collided with a static or dynamic object
	if (intersection > 0)
	{
		// Calculate the contact force
		glm::vec2 contactForce = 0.5f * (distance - (m_radius + pOther->getRadius()))*delta / distance;

		// Nudge the sphere that is not kinematic
		if (!m_isKinematic && !pOther->isKinematic())
		{
			// move each shape away in the direction of penetration
			m_position += contactForce;
			pOther->m_position -= contactForce;
		}
		else if (!m_isKinematic)
			m_position += contactForce * 2.0f;
		else if (!pOther->isKinematic())
			pOther->m_position -= contactForce * 2.0f;

		// This is used to check for victory condition for atom bombing game
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

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to check sphere to plane collision
// It then calls resolveCollision function if a collision is detected
// It takes in the following arguments:
// Plane* pOther is a pointer reference to the plane it's checking collision with
////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to check sphere to box collision
// It calls the CollideWithSphere of the box
// It takes in the following arguments:
// Box* pOther is a pointer reference to the box it's checking collision with
////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sphere::CollideWithBox(Box * pOther)
{
	pOther->CollideWithSphere(this);
}