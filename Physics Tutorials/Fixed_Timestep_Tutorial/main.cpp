#include "Fixed_Timestep_TutorialApp.h"

int main() {
	
	// allocation
	auto app = new Fixed_Timestep_TutorialApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}