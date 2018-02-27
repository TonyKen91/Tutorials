#pragma once
#include "PhysicsObject.h"

class Rigidbody;

class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec2 normal, float distance, float elasticity = 1);
	~Plane();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) {}
	virtual void debug() {}
	virtual void draw();
	virtual void resetPosition();

	void resolveCollision(Rigidbody* actor2, glm::vec2 contact);

	glm::vec2 getNormal() { return m_normal; }
	float getDistance() { return m_distanceToOrigin; }

	virtual void Collide(PhysicsObject* obj) { obj->CollideWithPlane(this); }

	virtual void CollideWithSphere(Sphere* pOther);
	virtual void CollideWithPlane(Plane* pOther);
	virtual void CollideWithBox(Box* pOther);

	float getTotalEnergy() { return 0; }
	virtual float getLinearKineticEnergy() { return 0; }
	virtual float getRotationalKineticEnergy() { return 0; }



protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin;
	float m_elasticity;
};

