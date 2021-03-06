#include "Box.h"
#include "Plane.h"
#include "Sphere.h"

#include <iostream>



/////////////////////////////////////////////////////////////////////////////////////////////////
// This is box's constructor which creates the instance of box
// It takes in the following arguments:
// glm::vec2 position which is the location of the box in reference to the Gizmos' origin
// glm::vec2 velocity which determines the box's initial velocity
// float mass which is the mass of the box which is used in physics calculation
// float height which determines the height of the box
// float width which determines the width of the box
// glm::vec4 colour uses a RGBA to determine the colour and opacity of the object
// float rotation which states the initial orientation
// float elasticity defines how elastic it is which determine its bounce
// float linearDrag which is used to reduce the linear velocity
// float angularDrag which is used to dampen the angular velocity of the sphere
/////////////////////////////////////////////////////////////////////////////////////////////////
Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, float height, float width,
 glm::vec4 colour, 
	float rotation, float elasticity, float linearDrag, float angularDrag) : 
	Rigidbody(BOX, position, velocity, mass, rotation, elasticity, linearDrag, angularDrag)
{
	// This create half extents of the width and height
	m_extents.x = width / 2;
	m_extents.y = height / 2;
	// Stores the box parameters
	m_width = width;
	m_height = height;
	m_colour = colour;
	// Calculate inertia of the object
	m_inertia = 1.0f / 12.0f * mass * width * height;
	// This stores the original inertia of the object
	m_originalInertia = m_inertia;
}


Box::~Box()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function handles the physics calculation for box and updates its properties
// This also calculate the local axes of the object
// It takes in the following arguments:
// glm::vec2 gravity which is the acceleration due to gravity
// float timeStep which is the timeStep that defines the frame rate of the scene
////////////////////////////////////////////////////////////////////////////////////////////////////////
void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::fixedUpdate(gravity, timeStep);

	// store the local axes
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to check box to sphere collision
// It then calls resolveCollision function if a collision is detected
// It takes in the following arguments:
// Sphere* pOther is a pointer reference to the second sphere it's checking collision with
////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		for (float y = -h2; y <= h2; y += m_height)
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
	// get the local position of the circle centre
	glm::vec2 localPos(glm::dot(m_localX, circlePos), glm::dot(m_localY, circlePos));

	// Checks all sides of the boxes where the potential penetration occurred
	if (localPos.y < h2 && localPos.y > -h2)
	{
		if (localPos.x > 0 && localPos.x < w2 + pOther->getRadius())
		{
			numContacts++;
			contact += glm::vec2(w2, localPos.y);
			direction = new glm::vec2(m_localX);
		}
		if (localPos.x < 0 && localPos.x > -(w2 + pOther->getRadius()))
		{
			numContacts++;
			contact += glm::vec2(-w2, localPos.y);
			direction = new glm::vec2(-m_localX);
		}
	}

	if (localPos.x < w2 && localPos.x > -w2)
	{
		if (localPos.y > 0 && localPos.y < h2 + pOther->getRadius())
		{
			numContacts++;
			contact += glm::vec2(localPos.x, h2);
			direction = new glm::vec2(m_localY);
		}
		if (localPos.y < 0 && localPos.y > -(h2 + pOther->getRadius()))
		{
			numContacts++;
			contact += glm::vec2(localPos.x, -h2);
			direction = new glm::vec2(-m_localY);
		}
	}


	if (numContacts > 0)
	{
		// average, and convert back into world coordinates
		contact = m_position + (1.0f / numContacts) * (m_localX * contact.x + m_localY * contact.y);

		// with the contact point we can find a penetration vector
		float pen = pOther->getRadius() - glm::length(contact - pOther->getPosition());

		glm::vec2 penVec = glm::normalize(contact - pOther->getPosition()) * pen;

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		// move each shape away in the direction of penetration depending on if they are static or kinematic
		if (!m_isKinematic && !pOther->isKinematic())
		{
			m_position += penVec * 0.5f;
			pOther->setPosition(pOther->getPosition() - penVec * 0.5f);
		}
		else if (!m_isKinematic)
			m_position += penVec;
		else if (!pOther->isKinematic())
		{
			pOther->setPosition(pOther->getPosition() - penVec);
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////

		// Calls resolve collision
		resolveCollision(pOther, contact, direction);
	}

	delete direction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to check box to plane collision
// It then calls resolveCollision function if a collision is detected
// It takes in the following arguments:
// Plane* pOther is a pointer reference to the plane it's checking collision with
////////////////////////////////////////////////////////////////////////////////////////////////////////
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
			if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane > 0) ||
				(distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane < 0))
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

		nudge(-pOther->getNormal() * penetration);
		//m_position -= pOther->getNormal() * penetration;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to check box to box collision
// It then calls resolveCollision function if a collision is detected
// It takes in the following arguments:
// Box* pOther is a pointer reference to the box it's checking collision with
////////////////////////////////////////////////////////////////////////////////////////////////////////
void Box::CollideWithBox(Box* pOther)
{
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
		// apply contact forces
		glm::vec2 displacement = pen * norm;


		if (!m_isKinematic && !pOther->isKinematic())
		{
			// move each shape away in the direction of penetration
			nudge(-displacement * 0.5f);
			pOther->nudge(displacement * 0.5f);
		}
		else if (!m_isKinematic)
			nudge(-displacement);
		else if (!pOther->isKinematic())
			pOther->nudge(displacement);


		resolveCollision(pOther, contact / float(numContacts), &norm);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to check box corners for box to box collision
// It takes in the following arguments:
// Box* pOther is a pointer reference to the box it's checking collision with
// Box& box is a direct reference to the box it's checking collision with
// glm::vec2& contact which is where this function going to store the contact points between the 2 boxes
// int& numContacts this states the number of contact between the boxes
// float& pen is used to store the depth of penetration in the local axis where the shallowest penetration occurred between the two boxes
// glm::vec2& edgeNormal which stores the edge which is normal to the penetration of the boxes
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Box::checkBoxCorners(const Box & box, glm::vec2 & contact, int & numContacts, float & pen, glm::vec2 & edgeNormal)
{

	float minX = 0, maxX = 0, minY = 0, maxY = 0;
	
	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);
	bool first = true;


	/////////////////////////////////////////////////////////

	for (float x = -m_extents.x; x < m_width; x += m_width)
	{
		for (float y = -m_extents.y; y < m_height; y += m_height)
		{
			// position of the second box in worldspace
			glm::vec2 p = box.m_position + x * box.m_localX + y * box.m_localY;
			// position in box's space
			glm::vec2 p0(glm::dot(p - m_position, m_localX), glm::dot(p - m_position, m_localY));


			// This part is used to set the maximum and minimum X and Y
			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;

			if (p0.x >= -box.m_extents.x && p0.x <= box.m_extents.x && p0.y >= -box.m_extents.y && p0.y <= box.m_extents.y)
			{
				numLocalContacts ++ ;
				localContact += p0;
			}
			first = false;
		}
	}

	// These parts return false if no penetration occurred between the two boxes
	if (maxX <= -box.m_extents.x || minX >= box.m_extents.x || maxY <= -box.m_extents.y || minY >= box.m_extents.y)
		return false;
	if (numLocalContacts == 0)
		return false;

	bool res = false;

	// Adds the contact points and average them out
	contact += m_position + (localContact.x * m_localX + localContact.y * m_localY) / (float)numLocalContacts;
	numContacts++;


	// This is used to search for the minimum penetration
	float pen0 = box.m_extents.x - minX;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = m_localX;
		pen = pen0;
		res = true;
	}
	
	pen0 = maxX + box.m_extents.x;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = -m_localX;
		pen = pen0;
		res = true;
	}
	
	pen0 = box.m_extents.y - minY;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = m_localY;
		pen = pen0;
		res = true;
	}
	
	pen0 = maxY + box.m_extents.y;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = -m_localY;
		pen = pen0;
		res = true;
	}
	return res;

	///////////////////////////////////////////////////////

}



////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to draw the box on the scene
// This uses 2 triangles to make up the box
////////////////////////////////////////////////////////////////////////////////////////////////////////
void Box::draw()
{
	// draw using local axes
	// This part finds the 4 corner points of a quadrilateral
	glm::vec2 p1 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p2 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p3 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p4 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;

	// The points found earlier are then used in creating two triangles that make up the quadrilateral
	aie::Gizmos::add2DTri(p1, p3, p2, m_colour);
	aie::Gizmos::add2DTri(p3, p1, p4, m_colour);
}

