#pragma once
#include <glm\ext.hpp>

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"

#include "Sphere.h"
#include "Plane.h"
#include "Box.h"

using glm::vec2;
using glm::vec4;

class Fixed_Timestep_TutorialApp : public aie::Application {
public:

	Fixed_Timestep_TutorialApp();
	virtual ~Fixed_Timestep_TutorialApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;
};