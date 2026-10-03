

#include <iostream>
#include<memory>

#include"framework/Application.h"

int main() {

	
std::unique_ptr<saga::Application> app = std::make_unique<saga::Application>();

app->run(); 




	


	return 0;

} // End main()
