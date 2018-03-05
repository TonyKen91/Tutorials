#include "Rigidbody.h"
#include "PhysicsScene.h"
#include <iostream>

#define MIN_LINEAR_THRESHOLD 0.2f
#define MIN_ROTATION_THRESHOLD 0.01f

/////////////////////////////////////////////////////////////////////////////////////////////////
// This is rigidbody's constructor which creates the instance of rigidbody
// It takes in the following arguments:
// ShapeType shapeID which specify which type of rigidbody it is
// glm::vec2 position which is the location of the rigidbody in reference to the Gizmos' origin
// glm::vec2 velocity which determines the rigidbody's initial velocity
// float mass which is the mass of the rigidbody which is used in physics calculation
// float rotation which states the initial orientation
// float elasticity defines how elastic it is which determine its bounce
// float linearDrag which is used to reduce the linear velocity
// float angularDrag which is used to dampen the angular velocity of the rigidbody
/////////////////////////////////////////////////////////////////////////////////////////////////
Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float mass, float rotation, float elasticity, float linearDrag, float angularDrag) :
	PhysicsObject(shapeID), m_position (position), m_velocity (velocity), m_rotation(rotation), m_mass(mass), m_elasticity(elasticity), m_linearDrag(linearDrag), m_angularDrag(angularDrag)
{
	m_angularVelocity = 0;
	m_isKinematic = false;
	m_originalMass = m_mass;
}


Rigidbody::~Rigidbody()
{
}


//////////////////////////////////////////////////////////////////////////////////////////
// This function is updates the physics of the object
// It takes in the following arguments:
// glm::vec2 gravity which is the acceleration due to gravity
// float timeStep which is the timeStep that defines the frame rate of the scene
//////////////////////////////////////////////////////////////////////////////////////////
void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	// Checks if the object is kinematic and if it is then it turns velocities off and masses to max and exits the function
	if (m_isKinematic)
	{
		m_velocity = { 0,0 };
		m_angularVelocity = 0;
		m_mass = INT_MAX;
		m_inertia = INT_MAX;
		return;
	}

	// Checks if the object is despawnable
	if (m_despawnable)
	{
		// If object is not in non-despawnable area, reduce timer
		if (glm::distance(m_position, m_scene->centreSphere->getPosition()) > m_scene->restrictedRadius - 20)
		{
			// if the object is despawnable then it updates the despawnTimer every frame
			m_despawnTimer -= timeStep;
		}

		// if the despawn timer have reach zero, it will then add the object to the remove list of the scene it is in
		if (m_despawnTimer <= 0)
		{
			m_scene->addToRemoveList(this);
		}
	}

	// adds drag into the velocity
	m_velocity *= (1 - m_linearDrag * timeStep);
	// adds angular velocity to the rotation of the object
	m_rotation += m_angularVelocity * timeStep;
	// reduces the angular velocity using the angular drag
	m_angularVelocity -= m_angularVelocity *m_angularDrag * timeStep;

	// applies sleep if applicable
	applySleepThreshold(gravity, timeStep);

	// updates position and velocity
	m_position += m_velocity * timeStep;
	m_velocity += gravity*timeStep;

	// reapplies sleep if possible
	applySleepThreshold(gravity, timeStep);

}

//////////////////////////////////////////////////////////////////////////////////////////
// This function is apply force to the rigidbody
// It takes in the following arguments:
// glm::vec2 force is the force that will be applied to the body
// glm::vec2 pos is the position of the force in relation to the centre mass of the body
//////////////////////////////////////////////////////////////////////////////////////////
void Rigidbody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	// These calculation calculates instantaneous velocity and angular velocity which are then added to the cumulative velocities
	m_velocity += force / m_mass;
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / m_inertia; // This adds torque/moment x and y component to the angular velocity per frame
}

/////////////////////////////////////////////////////////////////////////////////
// This function is used to resolve collision between objects
// It takes in the following arguments:
// Rigidbody* actor2 is a pointer reference to the second object
// glm::vec2 contact is the contact point of the two actors
// glm::vec2* collisionNormal is the normal of the collision
/////////////////////////////////////////////////////////////////////////////////
void Rigidbody::resolveCollision(Rigidbody * actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	// find the vector between their centres or use the provided direction of force
	glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(actor2->m_position - m_position);

	// get the vector perpendicular to the collision normal
	glm::vec2 perp(normal.y, -normal.x);

	// get the contact point when the collision occurs

	// determine the total velocity of the contact points for the two objects, for both linear and rotational

	// 'r' is the radius from axis to application of force
	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);

	// velocity of the contact point on this object
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;

	// velocity of contact point on actor2
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2) // they're moving closer
	{
		// calculate the effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_inertia);
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2 * r2) / actor2->m_inertia);

		float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		// apply equal and opposite forces
		applyForce(-force, contact - m_position);
		actor2->applyForce(force, contact - actor2->m_position);
	}
}

/////////////////////////////////////////////////////////////////////////////////
// This function is used to turn on or off sleeping state of object
// It takes in glm::vec2 gravity argument and float timeStep
// Gravity is used to check if an object is just temporarily stationary
// Timestep is used for the same check as well
/////////////////////////////////////////////////////////////////////////////////
void Rigidbody::applySleepThreshold(glm::vec2 gravity, float timeStep)
{
	// Turns off velocity if the object is below sleeping threshold and not temporarily stationary
	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
		if (glm::length(m_velocity) < glm::length(gravity)*m_linearDrag*timeStep)
			m_velocity = glm::vec2(0, 0);
	// Turns angular velocity to 0 if it's below the sleeping threshold
	if (abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD)
		m_angularVelocity = 0;

}

/////////////////////////////////////////////////////////////////////////////////
// This function is used to convert from local to world coordinates
// It takes in glm::vec2 argument where the contact occured
/////////////////////////////////////////////////////////////////////////////////
glm::vec2 Rigidbody::toWorld(glm::vec2 contact)
{
	// This can be done by either using the m_localX and m_localY
	// or calculate it using the objects rotation variable
	// or use a translation matrix and multiply your local coordinate by this matrix

	// This is using the m_localX and m_localY
	return m_position + m_localX * contact.x + m_localY * contact.y;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to calculate and return the linear kinetic energy of the body
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
float Rigidbody::getLinearKineticEnergy()
{
	float linearKinetic = ((0.5f)*m_mass* glm::dot(m_velocity, m_velocity));
	return linearKinetic;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to calculate and return the potential gravitational energy of the body
// This function takes in glm::vec2 gravity which is the acceleration due to gravitational pull on the body
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
float Rigidbody::getPotentialGravitationalEnergy(glm::vec2 gravity)
{

	{return (-(m_mass)*glm::dot(gravity, m_position)); }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to calculate and return the total kinetic energy of the body
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
float Rigidbody::getTotalEnergy()
{
	float total = getLinearKineticEnergy() + getRotationalKineticEnergy();
	return total;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to set kinematics on or off
// This function takes in a bool argument called state which defines if the object is kinematic or not
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Rigidbody::setKinematic(bool state)
{ 
	// This is used to retain memory if the object is kinematic or not
	m_isKinematic = state;
	
	// If the object is not kinematic or kinematic is turned off then it resets the mass and inertia to original value
	if (state == false)
	{
		m_mass = m_originalMass;
		m_inertia = m_originalInertia;
	}
	// if the object is kinematic, this turns mass and inertia to infinity and its velocities to 0
	else
	{
		m_velocity = { 0,0 };
		m_angularVelocity = 0;
		m_mass = INT_MAX;
		m_inertia = INT_MAX;
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This is used to set the despawn timer
// This function takes in a float argument called timer which is timer of how long before the object despawns
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Rigidbody::setDespawnTimer(float timer)
{
	m_despawnable = true;
	m_despawnTimer = timer;
}
