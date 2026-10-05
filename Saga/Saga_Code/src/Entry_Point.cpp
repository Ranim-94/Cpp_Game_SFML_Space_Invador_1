


#include "../include/Entry_Point.h"

#include "framework/Application.h"


int main(){

	Application* app = Saga::Application::get_application();

	app->run();

	delete app;


		return 0;


}// End main()





