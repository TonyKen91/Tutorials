#pragma once
#include <string>
#include "Rigidbody.h"

class Sphere : public Rigidbody
{
public:
	Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour,
		float rotation = 0, float elasticity = 1, float linearDrag = 0.01f, float angularDrag = 0.01f);
	~Sphere();

	virtual void draw();

	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);

	virtual void Collide(PhysicsObject* obj) { obj->CollideWithSphere(this); }

	virtual void CollideWithSphere(Sphere* pOther);
	virtual void CollideWithPlane(Plane* pOther);
	virtual void CollideWithBox(Box* pOther);

	std::string m_name = "No Name";

protected:
	float m_radius;
	glm::vec4 m_colour;
	int m_segments = 50;
	int gridCollision = 0;
};

