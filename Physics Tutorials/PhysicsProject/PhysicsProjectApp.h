#pragma once
#include <glm\ext.hpp>

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"

#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Spring.h"

using glm::vec2;
using glm::vec4;

class PhysicsProjectApp : public aie::Application {
public:

	PhysicsProjectApp();
	virtual ~PhysicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;
	float				m_totalEnergy = 0;
};