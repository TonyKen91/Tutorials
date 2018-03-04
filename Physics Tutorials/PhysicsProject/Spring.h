#pragma once
#include "PhysicsObject.h"

class Rigidbody;

class Spring :
	public PhysicsObject
{
public:
	Spring(Rigidbody* body1, Rigidbody* body2, float springCoefficient, float damping = 0.1f, float breakPointMultiplier = 10.0f, float restLength = 0, glm::vec2 contact1 = glm::vec2(0, 0), glm::vec2 contact2 = glm::vec2(0,0));
	~Spring();


	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() {}
	virtual void draw();
	virtual float getTotalEnergy();
	virtual float getLinearKineticEnergy();
	virtual float getRotationalKineticEnergy();


	virtual void CollideWithSphere(Sphere* pOther) {}
	virtual void CollideWithPlane(Plane* pOther) {}
	virtual void CollideWithBox(Box* pOther) {}
	virtual void Collide(PhysicsObject* obj) {}


	void setBreakable(bool state, float breakPointMultiplier = 10.0f);


	ShapeType getShapeID() { return m_shapeID; }


private:
	Rigidbody* m_body1;
	Rigidbody* m_body2;

	glm::vec2 m_contact1;
	glm::vec2 m_contact2;

	float m_damping;
	float m_restLength;
	float m_springCoefficient;			// the restoring force
	float m_springForceCap;
	bool m_breakable = true;
	float m_breakPoint;
};

