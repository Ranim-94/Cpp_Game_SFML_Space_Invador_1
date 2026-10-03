
#include "framework/Application.h"


namespace saga {


	Application::Application():
	m_window{sf::VideoMode({800,600}),
	"Space Invador"}
	{



	} // End Application()


	void Application::run(){

		while(this->m_window.isOpen()){

		while(const std::optional window_event = this->m_window.pollEvent()){
		
			if(window_event->is<sf::Event::Closed>()){
				this->m_window.close();
			}
		
		
		} // End inner while()

	this->m_window.clear(sf::Color::Black);
	
	this->m_window.display();


	}// End outer while()



	} // End run()




} // End namespace saga
