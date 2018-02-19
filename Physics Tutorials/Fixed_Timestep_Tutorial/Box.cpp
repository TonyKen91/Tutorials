#include "Box.h"

#include "Plane.h"
#include "Sphere.h"



Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, float height, float width, glm::vec4 colour) : Rigidbody(BOX, position, velocity, 0, mass)
{
	m_extents.x = width / 2;
	m_extents.y = height / 2;
	m_width = width;
	m_height = height;
	m_colour = colour;
	m_inertia = 1.0f / 12.0f * mass * width * height;
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

	int numContacts = 0;
	glm::vec2 contact(0, 0);		// contact is in our box coordinates


	//////////////////////////////////////////////////////////////////////

	// check the four corners to see if any of them are inside the circle
	for (float x = -w2; x <= w2; x += m_width) 
	{
		for (float y = -h2; y <= h2; y += m_width)
		{
			glm::vec2 cornerPoint = x * m_localX + y * m_localY;
			glm::vec2 distCorner2Circle = cornerPoint - circlePos;
			if (distCorner2Circle.x * distCorner2Circle.x + distCorner2Circle.y * distCorner2Circle.y < pOther->getRadius() * pOther->getRadius())
			{
				numContacts++;
				contact += glm::vec2(x, y);
			}
		}
	}

	/////////////////////////////////////////////////////////////////////

	glm::vec2* direction = nullptr;
	// get th4e local position of the circle cnetre
	glm::vec2 localPos(glm::dot(m_localX, circlePos), glm::dot(m_localY, circlePos));
	if (localPos.y < h2 && localPos.y > -h2)
	{
		if (localPos.x > 0 && localPos.x < w2 + pOther->getRadius())
		{
			numContacts++;
			contact += glm::vec2(w2, localPos.y);
			direction = new glm::vec2(m_localX);
		}
	}

	if (numContacts > 0)
	{
		// average, and convert back into world coordinates
		contact = m_position + (1.0f / numContacts) * (m_localX * contact.x + m_localY * contact.y);
		resolveCollision(pOther, contact, direction);
	}

	delete direction;
}

void Box::CollideWithPlane(Plane* pOther)
{
	int numContacts = 0;
	glm::vec2 contact(0, 0);
	float contactV = 0;
	float radius = 0.5f * std::fminf(m_width, m_height);
	float penetration = 0;

	// which side is the centre of mass on
	glm::vec2 planeOrigin = pOther->getNormal() * pOther->getDistance();
	float comFromPlane = glm::dot(m_position - planeOrigin, pOther->getNormal());



	////////////////////////////////////////////////////////////////////

	// check all four corners to see if we've hit the plane
	for (float x = -m_extents.x; x < m_width; x += m_width)
	{
		for (float y = -m_extents.y; y < m_height; y += m_height)
		{
			// get the position of the corner in world space
			glm::vec2 cornerPoint = m_position + x * m_localX + y * m_localY;

			float distFromPlane = glm::dot(cornerPoint - planeOrigin, pOther->getNormal());

			// this is the total velocity of the point
			float velocityIntoPlane = glm::dot(m_velocity + m_angularVelocity * (-y * m_localX + x * m_localY), pOther->getNormal());

			// if this corner is on the opposite side from the COM, and moving further in, we need to resolve the collision
			if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane >= 0) ||
				(distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane <= 0))
			{
				numContacts++;
				contact += cornerPoint;
				contactV += velocityIntoPlane;

				if (comFromPlane >= 0)
				{
					if (penetration > distFromPlane)
						penetration = distFromPlane;
				}
				else
				{
					if (penetration < distFromPlane)
						penetration = distFromPlane;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////


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

		m_position -= pOther->getNormal() * penetration;
	}
}

void Box::CollideWithBox(Box* pOther)
{
	glm::vec2 boxPos = pOther->m_position - m_position;

	glm::vec2 norm(0, 0);
	glm::vec2 contact(0, 0);
	float pen = 0;
	int numContacts = 0;

	checkBoxCorners(*pOther, contact, numContacts, pen, norm);

	if (pOther->checkBoxCorners(*this, contact, numContacts, pen, norm))
	{
		norm = -norm;
	}
	if (pen > 0)
	{
		resolveCollision(pOther, contact / float(numContacts), &norm);
	}
}

bool Box::checkBoxCorners(const Box & box, glm::vec2 & contact, int & numContacts, float & pen, glm::vec2 & edgeNormal)
{

	float minX, maxX, minY, maxY;
	
	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);

	bool first = true;


	/////////////////////////////////////////////////////////

	for (float x = -m_extents.x; x < m_width; x += m_width)
	{
		for (float y = -m_extents.y; y < m_height; y += m_height)
		{
			// position in worldspace
			glm::vec2 p = m_position + x * m_localX + y * m_localY;
			// position in box's space
			glm::vec2 p0(glm::dot(p - m_position, m_localX), glm::dot(p - m_position, m_localY));


			// This part is used to set the maximum and minimum X and Y
			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;

			if (p0.x >= -m_extents.x && p0.x <= m_extents.x && p0.y >= -m_extents.y && p0.y <= m_extents.y)
			{
				numLocalContacts ++ ;
				localContact += p0;
			}
			first = false;
		}
	}

	if (maxX < -m_extents.x || minX > m_extents.x || maxY < -m_extents.y || minY > m_extents.y)
		return false;
	if (numContacts == 0)
		return false;

	bool res = false;

	contact += m_position + (localContact.x * m_localX + localContact.y * m_localY) / (float)numLocalContacts;
	numContacts++;

	float pen0 = m_extents.x = minX;

	if (pen > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = m_localX;
		pen = pen0;
		res = true;
	}
	pen0 = maxX + m_extents.x;
	
	if (pen > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = -m_localX;
		pen = pen0;
		res = true;
	}
	pen0 = m_extents.y - minY;
	
	if (pen > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = m_localY;
		pen = pen0;
		res = true;
	}
	pen0 = maxY + m_extents.y;
	
	if (pen > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = -m_localY;
		pen = pen0;
		res = true;
	}
	return res;

	///////////////////////////////////////////////////////

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


	aie::Gizmos::add2DTri(p1, p3, p2, m_colour);
	aie::Gizmos::add2DTri(p3, p1, p4, m_colour);
}

bool Box::checkCollision(PhysicsObject * pOther)
{
	return false;
}

