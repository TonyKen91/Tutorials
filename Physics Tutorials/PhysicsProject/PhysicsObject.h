#pragma once

#include <glm\vec2.hpp>
#include <glm\vec4.hpp>
#include <glm\ext.hpp>
#include <Gizmos.h>

class Sphere;
class Plane;
class Box;

enum ShapeType {
	JOINT = -1,
	PLANE = 0,
	SPHERE,
	BOX,
};

const int 	SHAPE_COUNT = 3;

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void draw() = 0;
	virtual void resetPosition() {};
	virtual float getTotalEnergy() = 0;
	virtual float getLinearKineticEnergy() = 0;
	virtual float getRotationalKineticEnergy() = 0;

	virtual void CollideWithSphere(Sphere* pOther) = 0;
	virtual void CollideWithPlane(Plane* pOther) = 0;
	virtual void CollideWithBox(Box* pOther) = 0;
	virtual void Collide(PhysicsObject* obj) = 0;

	~PhysicsObject();
	ShapeType getShapeID() { return m_shapeID; }

protected:
	ShapeType m_shapeID;
};

