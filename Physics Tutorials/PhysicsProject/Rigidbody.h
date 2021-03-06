#pragma once
#include "PhysicsObject.h"

class PhysicsScene;

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float mass, float rotation, float elasticity = 1, float linearDrag = 0.01f, float angularDrag = 0.01f);
	~Rigidbody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	void applyForce(glm::vec2 force, glm::vec2 pos);
	void resolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);
	void applySleepThreshold(glm::vec2 gravity, float timeStep);
	glm::vec2 toWorld(glm::vec2 contact);

	// Getter functions which are used to get properties of the rigid body
	glm::vec2 getPosition() { return m_position; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getRotation() { return m_rotation; }
	float getMass() { return (m_isKinematic) ? INT_MAX : m_mass; }
	float getElasticity() { return m_elasticity; }
	
	virtual float getTotalEnergy();
	virtual float getLinearKineticEnergy(); 
	virtual float getRotationalKineticEnergy() { return ((0.5f)*m_inertia * m_angularVelocity * m_angularVelocity); }
	float getPotentialGravitationalEnergy(glm::vec2 gravity);

	// Setter functions for rigid body's properties
	void setPosition(glm::vec2 position) { m_position = position; }
	virtual void setDespawnTimer(float timer);
	void setKinematic(bool state);

	void nudge(glm::vec2 dx) { m_position += dx; }
	bool isKinematic() { return m_isKinematic; }

protected:
	glm::vec2 m_position = { 0, 0 };
	glm::vec2 m_velocity = { 0, 0 };
	float m_rotation = 0; // 2D so we only need a single float to represent our rotation
	float m_mass = 0;

	float m_originalMass = 0;
	float m_originalInertia = 0;

	float m_linearDrag = 0;
	float m_angularDrag = 0;
	float m_angularVelocity = 0;
	float m_elasticity = 1;

	float m_gravity = 0;

	float m_inertia = 0;

	bool m_isKinematic;
	bool m_scoreGiven = false;

	float m_despawnTimer = 5.0f;
	bool m_despawnable = false;


	// store the local x, y axes of the rigidbody based on its angle of rotation
	glm::vec2 m_localX = { 0, 0 };
	glm::vec2 m_localY = { 0, 0 };

};

