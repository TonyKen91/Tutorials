#include "Spring.h"
#include "Rigidbody.h"
#include "PhysicsScene.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This is spring's constructor which creates the instance of spring
// It takes in the following arguments:
// Rigidbody * body1 is a pointer reference to the first body that the spring connects
// Rigidbody * body2 is a pointer reference to the second body that the spring connects
// float springCoefficient determines the strength of the spring to return it back to restLength
// float damping is the damping that reduces the oscillation on of the spring from spring coefficient
// float breakPointMultiplier is the constant multiplied to the restLength that determines the  breakpoint of the spring
// float restLength is the length of the spring at its rest state
// glm::vec2 contact1 is the contact point where the spring connects with the first body, default value is the centre of the body
// glm::vec2 contact2 is the contact point where the spring connects with the second body
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Spring::Spring(Rigidbody * body1, Rigidbody * body2, float springCoefficient, float damping, float breakPointMultiplier, float restLength, glm::vec2 contact1, glm::vec2 contact2) : 
	PhysicsObject(ShapeType::JOINT), m_body1 (body1), m_body2(body2), m_restLength(restLength), m_springCoefficient(springCoefficient), m_damping(damping), m_contact1(contact1), m_contact2(contact2)
{
	// checks if there is a specified length, otherwise use the current distance between the two bodies
	if (restLength <= 0)
	{
		m_restLength = glm::distance(body1->getPosition(), body2->getPosition());
	}
	m_breakPoint = breakPointMultiplier * m_restLength;
}

Spring::~Spring()
{
}

//////////////////////////////////////////////////////////////////////////////////////////
// This function is updates the physics of the object
// It takes in the following arguments:
// glm::vec2 gravity which is the acceleration due to gravity
// float timeStep which is the timeStep that defines the frame rate of the scene
//////////////////////////////////////////////////////////////////////////////////////////
void Spring::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	// Converts the contact points to world coordinates
	glm::vec2 p2 = m_body2->toWorld(m_contact2);
	glm::vec2 p1 = m_body1->toWorld(m_contact1);
	
	// Calculates the displacement and distance of the two bodies that the spring is connected to
	glm::vec2 dist = p2 - p1;
	float length = sqrtf(dist.x*dist.x + dist.y*dist.y);

	// apply damping
	glm::vec2 relativeVelocity = m_body2->getVelocity() - m_body1->getVelocity();

	// calculate the different in the current length and rest length of the spring
	float deltaLength = m_restLength - length;
	if (deltaLength > m_restLength)
		deltaLength = m_restLength;

	// Checks if the spring is breakable and if it exceeds its breakpoint
	if (m_breakable && length > m_breakPoint)
	{
		m_scene->addToRemoveList(this);
		m_scene->isGameOver = true;
		return;
	}

	// F = -kX -bv
	glm::vec2 force = dist * m_springCoefficient * (deltaLength) - m_damping * relativeVelocity;

	m_body1->applyForce(-force * timeStep, p1 - m_body1->getPosition());
	m_body2->applyForce(force * timeStep, p2 - m_body2->getPosition());

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to draw the spring on the scene
// This uses a line that connects the 2 bodies
////////////////////////////////////////////////////////////////////////////////////////////////////////
void Spring::draw()
{
	aie::Gizmos::add2DLine(m_body1->getPosition(), m_body2->getPosition(), glm::vec4(0.7f, 0.7f, 0.7f, 1));
}

// Total energy and the kinetic energy calculations are not accounted in total energy calculations
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to toggle breakability of a spring and using breakpointMultiplier to determine its breakpoint
// It takes in the following arguments
// bool state which specify if a spring is breakable or not
// float breakPointMultiplier is used to determine the breakpoint of the spring in relation to its restLength
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Spring::setBreakable(bool state, float breakPointMultiplier)
{
	m_breakable = state;
	if (m_breakable && breakPointMultiplier > 1)
		m_breakPoint = breakPointMultiplier*m_restLength;
}
