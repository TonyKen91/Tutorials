#include "PhysicsScene.h"


PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0,0))
{

}


PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}


void PhysicsScene::addActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
	if (actor->getShapeID() >=0)
		m_collidable.push_back(actor);
}

void PhysicsScene::PhysicsScene::removeActor(PhysicsObject* actor)
{
	m_actors.remove(actor);
	if (actor->getShapeID() >= 0)
	{
		m_collidable.remove(actor);

		// loop over all actors

		// if its a spring, check if actor is one of its ends

		// if so delete the spring
	}

	delete actor;
}

void PhysicsScene::update(float dt)
{
	static std::list<PhysicsObject*> dirty;


	// update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		totalEnergy = 0;
		linearKinetic = 0;
		rotationalKinetic = 0;
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
			totalEnergy += pActor->getTotalEnergy();
			linearKinetic += pActor->getLinearKineticEnergy();
			rotationalKinetic += pActor->getRotationalKineticEnergy();
		}
		accumulatedTime -= m_timeStep;

		// check for collisions (ideally you'd want to have some sort of
		// scene management in place)

		checkForCollision();

		//for (auto pActor : m_actors)
		//{
		//	for (auto pOther : m_actors)
		//	{
		//		if (pActor == pOther)
		//			continue;
		//		if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() &&
		//			std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
		//			continue;

		//		Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor);
		//		if (pRigid->checkCollision(pOther) == true)
		//		{
		//			//pRigid->applyForceToActor(dynamic_cast<Rigidbody*>(pOther),
		//			//	pRigid->getVelocity() * pRigid->getMass());
		//			dirty.push_back(pRigid);
		//			dirty.push_back(pOther);
		//		}
		//	}
		//}
		dirty.clear();
	}
}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->draw();
	}
}

//void PhysicsScene::debugScene()
//{
//	int count = 0;
//	for (auto pActor : m_actors)
//	{
//		cout << count << " : ";
//		pActor->debug();
//		count++;
//	}
//}

//typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
//
//static fn collisionFunctionArray[] =
//{
//	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere, PhysicsScene::plane2Box,
//	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere, PhysicsScene::sphere2Box,
//	PhysicsScene::box2Plane, PhysicsScene::box2Sphere, PhysicsScene::box2Box,
//};

void PhysicsScene::checkForCollision()
{
	//int actorCount = m_collidable.size();

	////need to check for collisions against all objects except this one
	//for (auto it = m_collidable.begin(); it!= std::prev(m_collidable.end()); it++)
	//{
	//	PhysicsObject* object1 = *it;
	//	for (auto it2 = std::next(it); it2 != m_collidable.end(); it2++)
	//	{
	//		PhysicsObject* object2 = *it2;

	//		if (object1->getShapeID() < 0 || object2->getShapeID() < 0)
	//			continue;

	//		// I think this handles the collision between objects with a joint but we'll see
	//		// Might need to add collision handling between objects connected with joints


	//		object1->Collide(object2);



	 //The unmodified version

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







			//int shapeId1 = object1->getShapeID();
			//int shapeId2 = object2->getShapeID();

			//// using function pointers
			//int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			//fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			//if (collisionFunctionPtr != nullptr)
			//{
			//	// did a collision occur
			//	collisionFunctionPtr(object1, object2);
			//}

		}
	}


}
//
//bool PhysicsScene::plane2Plane(PhysicsObject *, PhysicsObject *)
//{
//	return false;
//}
//
//bool PhysicsScene::plane2Sphere(PhysicsObject *, PhysicsObject *)
//{
//	return false;
//}
//
//bool PhysicsScene::plane2Box(PhysicsObject * obj1, PhysicsObject * obj2)
//{
//	return false;
//}
//
//bool PhysicsScene::sphere2Plane(PhysicsObject *, PhysicsObject *)
//{
//	return false;
//}
//
//bool PhysicsScene::sphere2Sphere(PhysicsObject *obj1, PhysicsObject *obj2)
//{
//	// try to cast objects to sphere and sphere
//	Sphere *sphere1 = dynamic_cast<Sphere*>(obj1);
//	Sphere *sphere2 = dynamic_cast<Sphere*>(obj2);
//	// if we are successful then test for collision
//	if (sphere1 != nullptr && sphere2 != nullptr)
//	{
//
//	}
//
//	return false;
//}
//
//bool PhysicsScene::sphere2Box(PhysicsObject * obj1, PhysicsObject * obj2)
//{
//	std::cout << "Not suppose to be here" << std::endl;
//	return false;
//}
//
//bool PhysicsScene::box2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
//{
//	std::cout << "Not suppose to be here" << std::endl;
//	return false;
//}
//
//bool PhysicsScene::box2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
//{
//	std::cout << "Not suppose to be here" << std::endl;
//	return false;
//}
//
//bool PhysicsScene::box2Box(PhysicsObject * obj1, PhysicsObject * obj2)
//{
//	std::cout << "Not suppose to be here" << std::endl;
//	return false;
//}

//void RemoveFromVector(std::vector<int>& vec, int entry)
//{
//	for (auto it = vec.begin(); it != vec.end(); it++)
//	{
//		int x = *it;
//		if (x == entry)
//		{
//			it = vec.erase(it);
//			// quick manual check if we've just erased the last element
//			if (it == vec.end())
//				break;
//		}
//	}
//
//	// or this and requires algorithm library
//	vec.erase(std::remove(vec.begin(), vec.end(), entry), vec.end());
//}