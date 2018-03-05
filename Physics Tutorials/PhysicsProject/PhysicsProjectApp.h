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
	aie::Font*			m_menuFont;
	aie::Font*			m_UIConditionFont;

	PhysicsScene*		m_gameScene;
	PhysicsScene*		m_testScene;
	float				m_totalEnergy = 0;

private:
	const float			m_aspectRatio = 16 / 9.0f;
	float				m_gameTimer = 200.0f;
	int					m_pageNumber = 0;
};