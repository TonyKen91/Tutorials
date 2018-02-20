#pragma once
#include "PhysicsObject.h"

class PhysicsScene;

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity = 1, float linearDrag = 0.01f, float angularDrag = 0.01f);
	~Rigidbody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec2 force, glm::vec2 pos);
	//void applyForceToActor(Rigidbody* actor2, glm::vec2 force);
	void resolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);

	virtual bool checkCollision(PhysicsObject* pOther) = 0;

	glm::vec2 getPosition() { return m_position; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getRotation() { return m_rotation; }
	float getMass() { return m_mass; }

	float getLinearKineticEnergy() { return ((1 / 2)*m_mass* length(m_velocity)*length(m_velocity)); }
	float getRotationalKineticEnergy() { return ((1 / 2)*m_inertia * m_angularVelocity * m_angularVelocity); }
	float getPotentialGravitationalEnergy(glm::vec2 gravity);

	float getElasticity() { return m_elasticity; }

	void setPosition(glm::vec2 position) { m_position = position; }

	float getTotalEnergy();

protected:
	glm::vec2 m_position = { 0, 0 };
	glm::vec2 m_velocity = { 0, 0 };
	float m_rotation = 0; // 2D so we only need a single float to represent our rotation
	float m_mass = 0;

	float m_linearDrag = 0;
	float m_angularDrag = 0;
	float m_angularVelocity = 0;
	float m_elasticity = 1;

	float m_gravity = 0;

	float m_inertia = 1;

	bool m_isKinematic = false;
};

