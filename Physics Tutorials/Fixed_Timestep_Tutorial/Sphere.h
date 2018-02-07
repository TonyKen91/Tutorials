#pragma once
#include "Rigidbody.h"
class Sphere : public Rigidbody
{
public:
	Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
	~Sphere();

	virtual void draw();
	virtual bool checkCollision(PhysicsObject* pOther);

	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }

	virtual void Collide(PhysicsObject* obj) { obj->CollideWithSphere(this); }

	virtual void CollideWithSphere(Sphere* pOther) { pOther->CollideWithSphere(this); }

	//virtual void CollideWithObj(PhysicsObject* obj) { obj->CollideWithSphere(this); }
	//virtual void CollideWithSphere(Sphere * sphere);

protected:
	float m_radius;
	glm::vec4 m_colour;
};

