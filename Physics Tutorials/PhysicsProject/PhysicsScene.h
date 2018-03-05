#pragma once

#include <glm\vec2.hpp>
#include <iostream>
#include <list>

#include "Sphere.h"
#include "Rigidbody.h"

using std::cout;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);
	void update(float dt);
	void updateGizmos();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

	void checkForCollision();

	void addToRemoveList(PhysicsObject* actor);

	bool isGameOver = false;

	float totalEnergy;
	float linearKinetic;
	float rotationalKinetic;

protected:

	glm::vec2 m_gravity;
	float m_timeStep;
	std::list<PhysicsObject*> m_actors;
	std::list<PhysicsObject*> m_collidable;
	std::list<PhysicsObject*> m_deletedObjects;
};

