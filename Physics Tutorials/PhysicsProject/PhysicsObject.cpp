#include "PhysicsObject.h"
#include "PhysicsScene.h"


//PhysicsObject::PhysicsObject()
//{
//}


PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::setGameScene(PhysicsScene* scene)
{
	m_scene = scene;
}

