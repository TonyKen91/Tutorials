#pragma once
#include "Rigidbody.h"
class Box :
	public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float mass, float height, float width, glm::vec4 colour);
	~Box();

	virtual void draw();
	virtual bool checkCollision(PhysicsObject* pOther);

	glm::vec4 getColour() { return m_colour; }

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);

	virtual void Collide(PhysicsObject* obj) { obj->CollideWithBox(this); }

	virtual void CollideWithSphere(Sphere* pOther);
	virtual void CollideWithPlane(Plane* pOther);
	virtual void CollideWithBox(Box* pOther);

	bool checkBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float &pen, glm::vec2& edgeNormal);

	//virtual void CollideWithObj(PhysicsObject* obj) { obj->CollideWithSphere(this); }
	//virtual void CollideWithSphere(Sphere * sphere);

protected:
	float m_height;
	float m_width;

	glm::vec2 m_extents;

	glm::vec4 m_colour;

	// store the local x, y axes of the box based on its angle of rotation
	glm::vec2 m_localX;
	glm::vec2 m_localY;

};

