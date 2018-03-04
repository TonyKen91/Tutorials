#include "PhysicsScene.h"

// PhysicsScene constructor which initialises the timestep and gravity with default values
PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0,0))
{

}

// PhysicsScene destructor which deallocates the list of actors when it's done
PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Add Actor function is used to add actors into the scene
// This function takes the following argument
// Actor is a pointer to a physicsObject which the function used to update that specific object
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void PhysicsScene::addActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
	actor->setGameScene(this);
	// This is used to check if the actor is collidable and then add it to the collidable list if it is
	if (actor->getShapeID() >=0)
		m_collidable.push_back(actor);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Remove Actor function is used to remove actors into the scene
// This function takes the following argument
// Actor is a pointer to a physicsObject which the function used to identify which object to remove
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PhysicsScene::PhysicsScene::removeActor(PhysicsObject* actor)
{
	m_actors.remove(actor);
	// This is used to check if the actor is collidable and then remove it from the collidable list if it is
	if (actor->getShapeID() >= 0)
	{
		m_collidable.remove(actor);
	}

	delete actor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update function is used to update the scene and the actors in it
// This function takes the following argument
// float dt which is the deltaTime. This is the time step between frames 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PhysicsScene::update(float dt)
{
	// update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	// This is used to calculate all calculation within the timestep that didn't get calculate which might be caused by lags
	while (accumulatedTime >= m_timeStep)
	{
		// Resets the energy calculation for the next frame
		totalEnergy = 0;
		linearKinetic = 0;
		rotationalKinetic = 0;

		// This is used to update all actors in the scene using each actor's fixedUpdate since these actors are physics objects
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);

			// These takes and adds the energy from each actors
			totalEnergy += pActor->getTotalEnergy();
			linearKinetic += pActor->getLinearKineticEnergy();
			rotationalKinetic += pActor->getRotationalKineticEnergy();
		}
		accumulatedTime -= m_timeStep;

		// Anything that was added in the deleteObjects list are removed from the scene using this code
		for (auto pActor : m_deletedObjects)
			removeActor(pActor);
		m_deletedObjects.clear();

		// This calls the check for collision function
		checkForCollision();
	}



}

/////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to update the Gizmo which has renderer in it to draw the objects
// This calls the draw function for each object in the scene
/////////////////////////////////////////////////////////////////////////////////////////////////
void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->draw();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to check any collision between objects in the scene
// This function goes through each actor in the collidable list
/////////////////////////////////////////////////////////////////////////////////////////////////
void PhysicsScene::checkForCollision()
{

	int actorCount = m_collidable.size();

	//need to check for collisions against all objects except this one
	for (auto it = m_collidable.begin(); it!= std::prev(m_collidable.end()); it++)
	{
		PhysicsObject* object1 = *it;
		for (auto it2 = std::next(it); it2 != m_collidable.end(); it2++)
		{
			PhysicsObject* object2 = *it2;

			if (object1->getShapeID() < 0 || object2->getShapeID() < 0)
				continue;

			// I think this handles the collision between objects with a joint but we'll see
			// Might need to add collision handling between objects connected with joints

			object1->Collide(object2);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to add objects to the remove list
// This function takes in a PhysicsObject pointer which points to the object that needs to be remove from the scene
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PhysicsScene::addToRemoveList(PhysicsObject * actor)
{
	m_deletedObjects.push_back(actor);
}
