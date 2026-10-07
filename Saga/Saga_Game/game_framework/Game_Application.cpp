

#include "Game_Application.h"

namespace saga{


	Game_App::Game_App(){



	}// End Game_App()


	Application* get_application(){

		return new Game_App;

	}

} // End namespace saga

