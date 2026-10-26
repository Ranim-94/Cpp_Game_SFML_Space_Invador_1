

#include "Game_Application.h"
#include "framework/World.h"

namespace saga{


	Game_App::Game_App(){

		// loading some level
		LoadWorld<World>();

	}// End Game_App()


	Application* get_application(){

		return new Game_App;

	}

} // End namespace saga

