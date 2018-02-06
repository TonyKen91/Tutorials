#include <glm\ext.hpp>
#include <Gizmos.h>

#include "Fixed_Timestep_TutorialApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

Fixed_Timestep_TutorialApp::Fixed_Timestep_TutorialApp() {

}

Fixed_Timestep_TutorialApp::~Fixed_Timestep_TutorialApp() {

}

bool Fixed_Timestep_TutorialApp::startup() {
	
	// increase the 2d line count to maximize the number of objects we can draw 
	aie::Gizmos::create(225U, 255U, 35535U, 35535U);


	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(vec2(0, -0));
	m_physicsScene->setTimeStep(0.01f);

	Sphere* ball1 = new Sphere(vec2(0, 0), vec2(0, 0), 4.0f, 4, vec4(1, 0, 0, 1));
//	Sphere* ball2 = new Sphere(vec2(10, 0), vec2(0, 0), 1.0f, 1, vec4(0, 1, 0, 1));

	m_physicsScene->addActor(ball1);
//	m_physicsScene->addActor(ball2);

	ball1->applyForce(vec2(4, 0));
//	ball2->applyForce(vec2(0, 0));


	//Sphere* ball;
	//ball = new Sphere(vec2(-40, 0), vec2(10, 30), 3.0f, 1, vec4(1, 0, 0, 1));
	//m_physicsScene->addActor(ball);

	return true;
}

void Fixed_Timestep_TutorialApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void Fixed_Timestep_TutorialApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Fixed_Timestep_TutorialApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}