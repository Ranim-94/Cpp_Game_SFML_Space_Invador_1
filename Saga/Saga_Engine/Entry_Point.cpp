


#include "Entry_Point.h"

#include "framework/Application.h"


int main(){

	saga::Application* app = saga::get_application();

	app->run();

	delete app;


		return 0;


}// End main()





