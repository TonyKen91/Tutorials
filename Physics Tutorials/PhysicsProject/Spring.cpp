#include "Spring.h"
#include "Rigidbody.h"
#include "PhysicsScene.h"


Spring::Spring(Rigidbody * body1, Rigidbody * body2, float springCoefficient, float damping, float breakPointMultiplier, float restLength, glm::vec2 contact1, glm::vec2 contact2) : 
	PhysicsObject(ShapeType::JOINT), m_body1 (body1), m_body2(body2), m_restLength(restLength), m_springCoefficient(springCoefficient), m_damping(damping), m_contact1(contact1), m_contact2(contact2)
{
	if (restLength <= 0)
	{
		m_restLength = glm::distance(body1->getPosition(), body2->getPosition());
	}
	m_breakPoint = breakPointMultiplier * m_restLength;
}

Spring::~Spring()
{
}

void Spring::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	glm::vec2 p2 = m_body2->toWorld(m_contact2);
	glm::vec2 p1 = m_body1->toWorld(m_contact1);
	glm::vec2 dist = p2 - p1;
	float length = sqrtf(dist.x*dist.x + dist.y*dist.y)/*glm::length(dist)*/;

	// apply damping
	glm::vec2 relativeVelocity = m_body2->getVelocity() - m_body1->getVelocity();

	// float
	float deltaLength = m_restLength - length;
	if (deltaLength > m_restLength)
		deltaLength = m_restLength;

	if (m_breakable && length > m_breakPoint)
	{
		m_scene->addToRemoveList(this);
		std::cout << "You broke one of the springs therefore gameover" << std::endl;
		return;
	}

	// F = -kX -bv
	glm::vec2 force = dist * m_springCoefficient * (deltaLength) - m_damping * relativeVelocity;

	m_body1->applyForce(-force * timeStep, p1 - m_body1->getPosition());
	m_body2->applyForce(force * timeStep, p2 - m_body2->getPosition());

}

void Spring::draw()
{
	aie::Gizmos::add2DLine(m_body1->getPosition(), m_body2->getPosition(), glm::vec4(0.7f, 0.7f, 0.7f, 1));
}

float Spring::getTotalEnergy()
{
	return 0.0f;
}

float Spring::getLinearKineticEnergy()
{
	return 0.0f;
}

float Spring::getRotationalKineticEnergy()
{
	return 0.0f;
}

void Spring::setBreakable(bool state, float breakPointMultiplier)
{
	m_breakable = state;
	if (m_breakable && breakPointMultiplier > 1)
		m_breakPoint = breakPointMultiplier*m_restLength;
}
