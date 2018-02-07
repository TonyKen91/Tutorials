#pragma once
#include "PhysicsObject.h"
class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec2 normal, float distance);
	~Plane();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) {}
	virtual void debug() {}
	virtual void draw();
	virtual void resetPosition();

	glm::vec2 getNormal() { return m_normal; }
	float getDistance() { return m_distanceToOrigin; }

	virtual void Collide(PhysicsObject* obj) { obj->CollideWithPlane(this); }

protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin;
};

