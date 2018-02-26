#include <glm\ext.hpp>
#include <Gizmos.h>

#include "PhysicsProjectApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

PhysicsProjectApp::PhysicsProjectApp() {
	// plans

	// Finish all rotation tutorials
	// Possibly implement two types of box collision detection which are circular edge detection and middle point resolution
	// Energy calculation with potential energy set at 0
	//		this is used to make sure that there are not energy gain every time you turn on and off gravity
	// Pass in the length and width
	// Make stages for the project

}

PhysicsProjectApp::~PhysicsProjectApp() {

}

bool PhysicsProjectApp::startup() {

	// increase the 2d line count to maximize the number of objects we can draw 
	aie::Gizmos::create(225U, 255U, 35535U, 35535U);


	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 15);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(vec2(0, -10));
	m_physicsScene->setTimeStep(0.001f);

	Sphere* ball1 = new Sphere(vec2(-10, 10), vec2(0, 0), 1.0f, 1, vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere(vec2(10, 20), vec2(0, 0), 1.0f, 1, vec4(0, 1, 0, 1));
	Sphere* ball3 = new Sphere(vec2(0, 15), vec2(0, 0), 1.0f, 1, vec4(1, 1, 0, 1));
	Sphere* ball4 = new Sphere(vec2(10, 20), vec2(0, 0), 2.0f, 2, vec4(0, 1, 1, 1));

	Sphere* ball5 = new Sphere(vec2(0, 10), vec2(0, -200), 2.0f, 4, vec4(0.2f, 0.7f, 0.7f, 1));
	Sphere* ball6 = new Sphere(vec2(0, 20), vec2(0, 100), 2.0f, 4, vec4(0.5f, 0.1f, 0.5f, 1));

	Box* box1 = new Box(vec2(0, 0), vec2(0, -200), 10.0f, 10.0f, 10.0f, vec4(1, 0, 1, 1));
	Box* box2 = new Box(vec2(5, -40), vec2(2, 100), 10.0f, 10.0f, 10.0f, vec4(0.5f, 0.5f, 1, 1));
	Box* box3 = new Box(vec2(-20, -20), vec2(15, 11), 2.0f, 2.0f, 2.0f, vec4(0.5f, 0.5f, 1, 1));
	Box* box4 = new Box(vec2(20, -20), vec2(10, 15), 2.0f, 2.0f, 2.0f, vec4(0.8f, 0.5f, 0.2f, 1));
	Box* box5 = new Box(vec2(5, -40), vec2(0, 23), 13.0f, 13.0f, 13.0f, vec4(0.5f, 0.5f, 1, 1));


	Spring* spring1 = new Spring(ball1, ball2, 10, 10, 10);
	Spring* spring2 = new Spring(ball2, ball3, 10, 10, 10);

	ball1->setKinematic(true);
	box2->setKinematic(true);

	m_physicsScene->addActor(ball1);
	m_physicsScene->addActor(ball2);
	m_physicsScene->addActor(ball3);
	//m_physicsScene->addActor(ball4);

	//m_physicsScene->addActor(ball5);
	//m_physicsScene->addActor(ball6);
	
	m_physicsScene->addActor(box1);
	m_physicsScene->addActor(box2);
	m_physicsScene->addActor(box3);
	m_physicsScene->addActor(box4);
	//m_physicsScene->addActor(box5);

	m_physicsScene->addActor(spring1);
	m_physicsScene->addActor(spring2);


	//ball1->applyForce(vec2(100, -10), vec2(0, 0));
	//ball2->applyForce(vec2(-100, -10), vec2(0, 0));


	Plane* plane1 = new Plane(glm::vec2(0, 1), -50);
	Plane* plane2 = new Plane(glm::vec2(-1, 3), 30);
	Plane* plane3 = new Plane(glm::vec2(-5, 5), 20);
	Plane* plane4 = new Plane(glm::vec2(5, -3), 50);

	m_physicsScene->addActor(plane1);
	m_physicsScene->addActor(plane2);
	m_physicsScene->addActor(plane3);
	m_physicsScene->addActor(plane4);


	//Sphere* ball;
	//ball = new Sphere(vec2(-40, 0), vec2(10, 30), 3.0f, 1, vec4(1, 0, 0, 1));
	//m_physicsScene->addActor(ball);

	return true;
}

void PhysicsProjectApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsProjectApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();
	m_totalEnergy = m_physicsScene->totalEnergy;

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);


	char energy[32];
	sprintf_s(energy, 32, "Total Energy: %f", m_physicsScene->totalEnergy);
	m_2dRenderer->drawText(m_font, energy, 0, 20);

	// done drawing sprites
	m_2dRenderer->end();
}