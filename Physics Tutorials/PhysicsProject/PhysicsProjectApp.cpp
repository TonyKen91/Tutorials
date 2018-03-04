#include <glm\ext.hpp>
#include <Gizmos.h>

#include "PhysicsProjectApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

PhysicsProjectApp::PhysicsProjectApp() {
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

	// Initialise the game scene for the project and setting the gravity and timeStep for the physics calculation
	m_gameScene = new PhysicsScene();
	m_gameScene->setGravity(vec2(0, 0));
	m_gameScene->setTimeStep(0.01f);

	// Initialise the test scene for the project and setting the gravity and timeStep for the physics calculation
	m_testScene = new PhysicsScene();
	m_testScene->setGravity(vec2(0, 0));
	m_testScene->setTimeStep(0.01f);


	setBackgroundColour(0.5f, 0.5f, 0.5f, 1);


	///////////////////////////////////////////////////////////////////////
	// Game Scene Code Block

	// Variables to set the height and width of the grid in game
	const int height = 5;
	const int width = 5;

	// Array holder used to determine the position of each sphere in the grid
	// This is also used to determine which sphere needs to be connected by springs
	Sphere* sphereArray[height][width];

	// This nested for loop is used to create each spheres and let the sphereArray have a reference pointer to them
	// The amount of spheres needed are determined by the height and width mentioned previously and add them to the game scene
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			sphereArray[i][j] = new Sphere(vec2(i * 4, j * 4), vec2(0, 0), 1.0f, 1, vec4(0.7f, 0, 0.7f, 1));
			m_gameScene->addActor(sphereArray[i][j]);
		}
	}

	// This nested for loop is used to create springs between spheres in the grid then add them to the game scene
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Spring* springMaker;

			// This creates the horizontal springs with spring coefficient value of 100 and spring damping of 5
			if (j < width - 1)
			{
				springMaker = new Spring(sphereArray[i][j], sphereArray[i][j + 1], 100, 5);
				m_gameScene->addActor(springMaker);
			}

			// This creates the vertical springs with spring coefficient value of 100 and spring damping of 5
			if (i < height - 1)
			{
				springMaker = new Spring(sphereArray[i][j], sphereArray[i + 1][j], 100, 5);
				m_gameScene->addActor(springMaker);
			}

			// This creates the vertical springs with spring coefficient value of 100 and spring damping of 20
			if (j < width - 1 && i < height - 1)
			{
				springMaker = new Spring(sphereArray[i][j], sphereArray[i + 1][j + 1], 100, 20);
				m_gameScene->addActor(springMaker);
			}
			if (j >0 && i < height - 1)
			{
				springMaker = new Spring(sphereArray[i][j], sphereArray[i + 1][j - 1], 100, 20);
				m_gameScene->addActor(springMaker);
			}


			// Bend springs
			// Bend springs are used to keep the joints from bending over each other by adding another spring
			// this spring connects two bodies that are separated two bodies apart

			//This creates horizontal bend springs with spring coefficient value of 50 and spring damping of 20
			if (j < width - 2)
			{//50, 20
				springMaker = new Spring(sphereArray[i][j], sphereArray[i][j + 2], 50, 20);
				m_gameScene->addActor(springMaker);
			}

			//This creates vertical bend springs with spring coefficient value of 50 and spring damping of 20 
			if (i < height - 2)
			{
				springMaker = new Spring(sphereArray[i][j], sphereArray[i + 2][j], 50, 20);
				m_gameScene->addActor(springMaker);
			}

			// corner springs
			// Corner springs are used to keep the shape of the grid from deforming due to forces that causes the corners to fold onto the body
			// These springs connect two bodies that are diagonal extremities of the grid
			// This creates corner springs with spring coefficient value of 10 and spring damping of 10
			if (i == 0 && j == 0)
			{
				springMaker = new Spring(sphereArray[i][j], sphereArray[height - 1][width - 1], 10, 10);
				m_gameScene->addActor(springMaker);
				springMaker = new Spring(sphereArray[i][width - 1], sphereArray[height - 1][j], 10, 10);
				m_gameScene->addActor(springMaker);

			}
		}
	}


	//This is used to apply some initial movement to the grid
	sphereArray[0][0]->applyForce(vec2(100, 0), vec2(0));


	// This is used to set the border width and height of the game area using boxes
	const float borderWidth = 150;
	const float borderHeight = 100;

	// Side borders
	Box* border1 = new Box(vec2(-borderWidth / 2.0f, 0), vec2(0, 0), 10.0f, borderHeight, 5, vec4(1, 1, 1, 1));
	Box* border2 = new Box(vec2(borderWidth / 2.0f, 0), vec2(0, 0), 10.0f, borderHeight, 5, vec4(1, 1, 1, 1));

	// Top and bottom borders
	Box* border3 = new Box(vec2(0, borderHeight / 2.0f), vec2(0, 0), 10.0f, 5, borderWidth, vec4(1, 1, 1, 1));
	Box* border4 = new Box(vec2(0, -borderHeight / 2.0f), vec2(0, 0), 10.0f, 5, borderWidth, vec4(1, 1, 1, 1));

	// Set all the borders as kinematic
	border1->setKinematic(true);
	border2->setKinematic(true);
	border3->setKinematic(true);
	border4->setKinematic(true);

	// Add all the borders into the game scene
	m_gameScene->addActor(border1);
	m_gameScene->addActor(border2);
	m_gameScene->addActor(border3);
	m_gameScene->addActor(border4);




	///////////////////////////////////////////////////////////////////
	// Test Scene code block

	// This part of the code are used to create 6 balls with random position, velocity, mass, size and colour
	Sphere* ball1 = new Sphere(vec2(-10, 10), vec2(0, 0), 1.0f, 1, vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere(vec2(10, 20), vec2(0, 0), 1.0f, 1, vec4(0, 1, 0, 1));
	Sphere* ball3 = new Sphere(vec2(0, 15), vec2(0, 0), 1.0f, 1, vec4(1, 1, 0, 1));
	Sphere* ball4 = new Sphere(vec2(10, 20), vec2(0, 0), 2.0f, 2, vec4(0, 1, 1, 1));
	Sphere* ball5 = new Sphere(vec2(0, 10), vec2(0, -200), 2.0f, 4, vec4(0.2f, 0.7f, 0.7f, 1));
	Sphere* ball6 = new Sphere(vec2(0, 20), vec2(0, 100), 2.0f, 4, vec4(0.5f, 0.1f, 0.5f, 1));


	// This part of the code are used to create 4 boxes with random position, velocity, mass, height, width and colour
	Box* box1 = new Box(vec2(0, 0), vec2(0, -200), 10.0f, 10.0f, 10.0f, vec4(1, 0, 1, 1));
	Box* box2 = new Box(vec2(5, -40), vec2(2, 100), 10.0f, 10.0f, 10.0f, vec4(0.5f, 0.5f, 1, 1));
	Box* box3 = new Box(vec2(-20, -20), vec2(15, 11), 2.0f, 2.0f, 2.0f, vec4(0.5f, 0.5f, 1, 1));
	Box* box4 = new Box(vec2(20, -20), vec2(10, 15), 2.0f, 2.0f, 2.0f, vec4(0.8f, 0.5f, 0.2f, 1));

	// This part of the code is used to create springs that connects 3 balls
	Spring* spring1 = new Spring(ball1, ball2, 10, 10, 10);
	Spring* spring2 = new Spring(ball2, ball3, 10, 10, 10);

	// This set one of the balls and one of the boxes to be kinematic
	ball1->setKinematic(true);
	box2->setKinematic(true);

	// This add all of the objects into the test scene
	m_testScene->addActor(ball1);
	m_testScene->addActor(ball2);
	m_testScene->addActor(ball3);
	m_testScene->addActor(ball4);
	m_testScene->addActor(ball5);
	m_testScene->addActor(ball6);

	m_testScene->addActor(box1);
	m_testScene->addActor(box2);
	m_testScene->addActor(box3);
	m_testScene->addActor(box4);

	m_testScene->addActor(spring1);
	m_testScene->addActor(spring2);

	// This apply force to 2 of the balls
	ball1->applyForce(vec2(100, -10), vec2(0, 0));
	ball2->applyForce(vec2(-100, -10), vec2(0, 0));

	// This creates planes used in the test scene
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50);
	Plane* plane2 = new Plane(glm::vec2(-1, 3), 30);
	Plane* plane3 = new Plane(glm::vec2(-5, 5), 20);
	Plane* plane4 = new Plane(glm::vec2(5, -3), 50);

	// This adds the planes into the test scene
	m_testScene->addActor(plane1);
	m_testScene->addActor(plane2);
	m_testScene->addActor(plane3);
	m_testScene->addActor(plane4);


	return true;
}

// This function is called when the application is closed
void PhysicsProjectApp::shutdown() {
	
	// These are used to deallocate memories
	delete m_font;
	delete m_2dRenderer;
	delete m_gameScene;
	delete m_testScene;
}

void PhysicsProjectApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	// Clears the screen
	aie::Gizmos::clear();

	// Takes the coordinates of the mouse
	int mouseX, mouseY;
	input->getMouseXY(&mouseX, &mouseY);
	
	// convert screen coordinate to Gizmo coordinate
	glm::vec2 screenSize = { getWindowWidth(), getWindowHeight() };
	mouseX = (int)(mouseX * 200 / screenSize.x) - 100;
	mouseY = (int)(mouseY * 200 / (screenSize.y * aspectRatio) - 100/aspectRatio);

	// Updates the test scene
	//m_testScene->update(deltaTime);
	//m_testScene->updateGizmos();

	// Updates the game scene and Gizmo
	m_gameScene->update(deltaTime);
	m_gameScene->updateGizmos();

	// Spawns a ball in the game scene from the left
	if (input->wasKeyPressed(aie::INPUT_KEY_RIGHT))
	{
		Sphere* playerBall = new Sphere(vec2(mouseX, mouseY), vec2(100, 0), 10.0f, 1.9f, vec4(1, 0, 0, 1),0, 0.5f);
		playerBall->setDespawnTimer(20);
		m_gameScene->addActor(playerBall);
	}

	// Spawns a ball in the game scene from the right
	if (input->wasKeyPressed(aie::INPUT_KEY_LEFT))
	{
		Sphere* playerBall = new Sphere(vec2(mouseX, mouseY), vec2(-100, 0), 10.0f, 1.9f, vec4(1, 0, 0, 1), 0, 0.5f);
		playerBall->setDespawnTimer(20);
		m_gameScene->addActor(playerBall);
	}

	// Spawns a ball in the game scene from the top
	if (input->wasKeyPressed(aie::INPUT_KEY_DOWN))
	{
		Sphere* playerBall = new Sphere(vec2(mouseX, mouseY), vec2(0, -100), 10.0f, 1.9f, vec4(1, 0, 0, 1), 0, 0.5f);
		playerBall->setDespawnTimer(20);
		m_gameScene->addActor(playerBall);
	}
	
	// Spawns a ball in the game scene from the bottom
	if (input->wasKeyPressed(aie::INPUT_KEY_UP))
	{
		Sphere* playerBall = new Sphere(vec2(mouseX, mouseY), vec2(0, 100), 10.0f, 1.9f, vec4(1, 0, 0, 1), 0, 0.5f);
		playerBall->setDespawnTimer(20);
		m_gameScene->addActor(playerBall);
	}
	
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
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// These are used to show the total kinetic energy, linear kinetic energy and rotational kinetic energy
	char energy[50];
	sprintf_s(energy, 50, "Total Kinetic Energy: %f", m_gameScene->totalEnergy);
	m_2dRenderer->drawText(m_font, energy, 0, 20);

	sprintf_s(energy, 50, "Linear Kinetic Energy: %f", m_gameScene->linearKinetic);
	m_2dRenderer->drawText(m_font, energy, 0, 60);

	sprintf_s(energy, 50, "Rotational Kinetic Energy: %f", m_gameScene->rotationalKinetic);
	m_2dRenderer->drawText(m_font, energy, 0, 40);


	// done drawing sprites
	m_2dRenderer->end();
}