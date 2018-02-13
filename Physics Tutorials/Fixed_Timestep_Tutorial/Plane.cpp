#include "Plane.h"
#include "Sphere.h"
#include "Rigidbody.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;


Plane::Plane() : PhysicsObject(ShapeType::PLANE)
{
	m_distanceToOrigin = 0;
	m_normal = vec2(0, 1);
	m_elasticity = 1;
}

Plane::Plane(glm::vec2 normal, float distance, float elasticity) : PhysicsObject(ShapeType::PLANE), m_normal(normal), m_distanceToOrigin(distance), m_elasticity(elasticity)
{
	m_normal = glm::normalize(m_normal);
}


Plane::~Plane()
{
}

void Plane::draw()
{
	float lineSegmentLength = 300;
	vec2 centerPoint = m_normal * m_distanceToOrigin;

	// easy to rotate normal through 90 degrees around z

	vec2 parallel(m_normal.y, -m_normal.x);
	vec4 colour(1, 1, 1, 1);
	vec2 start = centerPoint + (parallel * lineSegmentLength);
	vec2 end = centerPoint - (parallel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, colour);
}

void Plane::resetPosition()
{
}

void Plane::resolveCollision(Rigidbody* actor2, glm::vec2 contact)
{
	// the plane isn't moving, so the relative v3elocity is just actor2's velocity
	glm::vec2 vRel = actor2->getVelocity();

	float elasticity = (1 + actor2->getElasticity()) / 2.0f;
	float j = (glm::dot(-(1 + elasticity) * (actor2->getVelocity()), m_normal)) / glm::dot(m_normal, m_normal * (1 / actor2->getMass()));

	glm::vec2  force = m_normal * j;

	actor2->applyForce(force, contact - actor2->getPosition());
}

void Plane::CollideWithSphere(Sphere * pOther)
{
	pOther->CollideWithPlane(this);
	//glm::vec2 collisionNormal = m_normal;

	//float sphereToPlane = glm::dot(pOther->getPosition() , m_normal) - m_distanceToOrigin;

	//// if the sphere is behind the plane then both need to be flipped
	//if (sphereToPlane < 0)
	//{
	//	collisionNormal *= -1;
	//	sphereToPlane *= -1;
	//}

	//float interesection = pOther->getRadius() - sphereToPlane;
	//if (interesection > 0)
	//	pOther->getVelocity *= 0;

}

void Plane::CollideWithPlane(Plane * pOther)
{
	// Collision detection for these planes is not required
}

void Plane::CollideWithBox(Box * pOther)
{
}
