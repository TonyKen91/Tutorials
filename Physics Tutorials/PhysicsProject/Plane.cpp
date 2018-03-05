#include "Box.h"
#include "Plane.h"
#include "Sphere.h"
#include "Rigidbody.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;

////////////////////////////////////////////////////////////////////////////////////////////////////
// This is plane's default constructor which creates the instance of plane using default parameters
////////////////////////////////////////////////////////////////////////////////////////////////////
Plane::Plane() : PhysicsObject(ShapeType::PLANE)
{
	m_distanceToOrigin = 0;
	m_normal = vec2(0, 1);
	m_elasticity = 1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
// This is plane's constructor which creates the instance of a plane
// It takes in the following arguments:
// glm::vec2 normal which determines the direction of the normal vector of the plane
// float distance which determines the plane's distance from the origin
// float elasticity defines how elastic it is
/////////////////////////////////////////////////////////////////////////////////////////////////
Plane::Plane(glm::vec2 normal, float distance, float elasticity) : PhysicsObject(ShapeType::PLANE), m_normal(normal), m_distanceToOrigin(distance), m_elasticity(elasticity)
{
	m_normal = glm::normalize(m_normal);
}


Plane::~Plane()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to draw the plane on the scene
////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plane::draw()
{
	float lineSegmentLength = 300;
	vec2 centerPoint = m_normal * m_distanceToOrigin;

	// easy to rotate normal through 90 degrees around z

	vec2 parallel(m_normal.y, -m_normal.x);

	// Default colour of the plane
	vec4 colour(1, 1, 1, 1);

	// This is used to determine the start and end point of the plane which Gizmos uses to draw it
	vec2 start = centerPoint + (parallel * lineSegmentLength);
	vec2 end = centerPoint - (parallel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, colour);
}


/////////////////////////////////////////////////////////////////////////////////
// This function is used to resolve collision between plane and a rigidbody
// It takes in the following arguments:
// Rigidbody* actor2 is a pointer reference to the second object
// glm::vec2 contact is the contact point of the two actors
/////////////////////////////////////////////////////////////////////////////////
void Plane::resolveCollision(Rigidbody* actor2, glm::vec2 contact)
{
	// the plane isn't moving, so the relative velocity is just actor2's velocity
	glm::vec2 vRel = actor2->getVelocity();

	float elasticity = (1 + actor2->getElasticity()) / 2.0f;
	float j = (glm::dot(-(1 + elasticity) * (actor2->getVelocity()), m_normal)) / glm::dot(m_normal, m_normal * (1 / actor2->getMass()));

	glm::vec2  force = m_normal * j;

	actor2->applyForce(force, contact - actor2->getPosition());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to check plane to sphere collision
// It calls the CollideWithPlane of the sphere
// It takes in the following arguments:
// Sphere* pOther is a pointer reference to the sphere it's checking collision with
////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plane::CollideWithSphere(Sphere * pOther)
{
	pOther->CollideWithPlane(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is just a dummy function inherited from PhysicsObject class
// It takes in the following arguments:
// Plane* pOther is a pointer reference to the plane it's checking collision with
////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plane::CollideWithPlane(Plane * pOther)
{
	// Collision detection for these planes is not required
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to check plane to box collision
// It calls the CollideWithPlane of the box
// It takes in the following arguments:
// Box* pOther is a pointer reference to the box it's checking collision with
////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plane::CollideWithBox(Box * pOther)
{
	pOther->CollideWithPlane(this);
}
