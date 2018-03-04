#include "PhysicsObject.h"
#include "PhysicsScene.h"


PhysicsObject::~PhysicsObject()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to give physics objects have a reference to the scene they are in
// This function takes the following argument:
// physicsScene pointer called scene which is the scene that the object exist in
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void PhysicsObject::setGameScene(PhysicsScene* scene)
{
	m_scene = scene;
}

