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
}

void PhysicsScene::PhysicsScene::removeActor(PhysicsObject* actor)
{
	m_actors.remove(actor);
}

void PhysicsScene::update(float dt)
{
	static std::list<PhysicsObject*> dirty;


	// update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;

		// check for collisions (ideally you'd want to have some sort of
		// scene management in place)

		for (auto pActor : m_actors)
		{
			for (auto pOther : m_actors)
			{
				if (pActor == pOther)
					continue;
				if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() &&
					std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
					continue;

				Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor);
				if (pRigid->checkCollision(pOther) == true)
				{
					//pRigid->applyForceToActor(dynamic_cast<Rigidbody*>(pOther),
					//	pRigid->getVelocity() * pRigid->getMass());
					dirty.push_back(pRigid);
					dirty.push_back(pOther);
				}
			}
		}
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

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		cout << count << " : ";
		pActor->debug();
		count++;
	}
}


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