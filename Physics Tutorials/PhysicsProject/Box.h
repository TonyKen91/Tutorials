#pragma once
#include "Rigidbody.h"
class Box :
	public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float mass, float height, float width, glm::vec4 colour,
		float rotation = 0, float elasticity = 1, float linearDrag = 0.01f, float angularDrag = 0.01f);
	~Box();

	virtual void draw();

	glm::vec4 getColour() { return m_colour; }

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);

	virtual void Collide(PhysicsObject* obj) { obj->CollideWithBox(this); }

	virtual void CollideWithSphere(Sphere* pOther);
	virtual void CollideWithPlane(Plane* pOther);
	virtual void CollideWithBox(Box* pOther);

	bool checkBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float &pen, glm::vec2& edgeNormal);

protected:
	float m_height = 0;
	float m_width = 0;

	glm::vec2 m_extents = { 0 ,0 };

	glm::vec4 m_colour = { 0, 0, 0, 0 };

};

