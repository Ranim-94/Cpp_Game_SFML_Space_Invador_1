

#include "Game_Application.h"
#include "framework/World.h"
#include "framework/Actor.h"

namespace saga{


	Game_App::Game_App(){

		// loading some level
		std::weak_ptr<World> new_world =  LoadWorld<World>();

		/* 
		   1) World is instantiated
		   			- new_world		
		*/


		// creating some temporary Actors and pushing them to a list
		new_world.lock()->Spawn_Actor<Actor>();

		// heap allocation of Actor list (temprary)
		// so the temporarly will be used to create the actual actor in the game

	}// End Game_App()


	Application* get_application(){

		return new Game_App;

	}

} // End namespace saga

