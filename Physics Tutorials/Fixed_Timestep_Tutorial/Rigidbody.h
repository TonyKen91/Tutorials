#pragma once
#include "PhysicsObject.h"
class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity = 1, float linearDrag = 0.01f, float angularDrag = 0.01f);
	~Rigidbody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec2 force, glm::vec2 pos);
	void applyForceToActor(Rigidbody* actor2, glm::vec2 force);
	void resolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);

	virtual bool checkCollision(PhysicsObject* pOther) = 0;

	glm::vec2 getPosition() { return m_position; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getRotation() { return m_rotation; }
	float getMass() { return m_mass; }

	float getKineticEnergy() { return ((1 / 2)*m_mass* length(m_velocity)*length(m_velocity)); }

	float getElasticity() { return m_elasticity; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_rotation; // 2D so we only need a single float to represent our rotation
	float m_mass;

	float m_linearDrag;
	float m_angularDrag;
	float m_angularVelocity;
	float m_elasticity;

	float m_inertia;

	bool m_isKinematic = false;
};

