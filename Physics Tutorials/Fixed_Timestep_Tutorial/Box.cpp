#include "Box.h"

#include "Plane.h"
#include "Sphere.h"



Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, float height, float width, glm::vec4 colour) : Rigidbody(BOX, position, velocity, 0, mass)
{
	m_extents.x = width / 2;
	m_extents.y = height / 2;
	m_width = width;
	m_height = height;
}


Box::~Box()
{
}

void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::fixedUpdate(gravity, timeStep);

	// store the local axes
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void Box::CollideWithSphere(Sphere* pOther)
{
	glm::vec2 circlePos = pOther->getPosition() - m_position;
	float w2 = m_width / 2, h2 = m_height / 2;
}

void Box::CollideWithPlane(Plane* pOther)
{
	int numContacts = 0;
	glm::vec2 contact(0, 0);
	float contactV = 0;
	float radius = 0.5f * std::fminf(m_width, m_height);

	// which side is the centre of mass on
	glm::vec2 planeOrigin = pOther->getNormal() * pOther->getDistance();
	float comFromPlane = glm::dot(m_position - planeOrigin, pOther->getNormal());

	// check all four corners to see if we've hit the plane
	for (float x = -m_extents.x; x < m_width; x += m_width)
	{
		for (float y = -m_extents.y; y < m_height; y += m_height)
		{
			// get the poisition of the corner in world space
			glm::vec2 cornerPoint = m_position + x * m_localX + y * m_localY;

			float distFromPlane = glm::dot(cornerPoint - planeOrigin, pOther->getNormal());

			// this is the total velocity of the point
			float velocityIntoPlane = glm::dot(m_velocity + m_angularVelocity * (-y * m_localX + x * m_localY), pOther->getNormal());

			// if this corner is on the opposite side from the COM, and moving further in, we need to resolve the collision
			if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane > 0) ||
				(distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane < 0))
			{
				numContacts++;
				contact += cornerPoint;
				contactV += velocityIntoPlane;
			}

		}
	}

	// we've had a hit - typically only two corners can contact
	if (numContacts > 0)
	{
		// get the average collision velocity into the plane
		// (covers linear and rotational velocity of all corners involved)
		float collisionV = contactV / (float)numContacts;

		// get the acceleration required to stop (restitution = 0) or reverse
		// (restitution = 1) the average velocity into the plane
		glm::vec2 acceleration = -pOther->getNormal() * ((1.0f + m_elasticity) * collisionV);

		// and the average position at which we'll apply the force
		// (corner or edge centre)
		glm::vec2 localContact = (contact / (float)numContacts) - m_position;

		// this is the perpendicular distance we apply the force at relative to the COM, so Torque = F*r
		float r = glm::dot(localContact, glm::vec2(pOther->getNormal().y, -pOther->getNormal().x));

		// work out the "effective mass" - this is a combination of moment of inertia and mass,
		// and tells us how much the contact point velocity will change with hte force we're applying
		float mass0 = 1.0f / (1.0f / m_mass + (r*r) / m_inertia);

		// and apply the force
		applyForce(acceleration * mass0, localContact);

	}
}

void Box::CollideWithBox(Box* pOther)
{
}




void Box::draw()
{
	// if only using rotation
	// glm::mat4 transofrm = glm::rotate(m_rotation, glm::vec3(0, 0, 1));
	// aie::Gizmos::add2DAABBFilled(getCenter(), m_extents, m_colour, &transform);

	// draw using local axes
	glm::vec2 p1 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p2 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p3 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p4 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;

	aie::Gizmos::add2DTri(p1, p2, p3, m_colour);
	aie::Gizmos::add2DTri(p3, p4, p1, m_colour);
}

bool Box::checkCollision(PhysicsObject * pOther)
{
	return false;
}

