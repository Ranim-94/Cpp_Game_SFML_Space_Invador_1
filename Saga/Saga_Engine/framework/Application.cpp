
#include "framework/Application.h"

#include<iostream>
#include<format>


namespace saga {


	Application::Application():
	m_window{sf::VideoMode({800,600}),"Space Invador"},
	m_frame_rate_target{60.0},
	m_tick_clock{}
	{



	} // End Application()


	void Application::run(){

		this->m_tick_clock.restart();

		float time_accumulated = 0.f; 

		// computing target frame time
		float time_frame = 1.0f/this->m_frame_rate_target;


		while(this->m_window.isOpen()){

			// Checking for events
			while(const std::optional window_event = this->m_window.pollEvent()){
			
				if(window_event->is<sf::Event::Closed>()){
					this->m_window.close();
				}

			} // End inner while() checking for events

		this->render_internal();// display things like shapes (rectangle,circle,....)		
		
		// measure 'time_accumulated' 
		time_accumulated += this->m_tick_clock.restart().asSeconds();

		// udapte time if condition is true	
		while(time_accumulated >= time_frame){

			/*
			This while() runs at a fixed 
			frame_rate since it only enter 
			when "time_accumulated" is big enough
		
		*/

			time_accumulated -= time_frame;

			this->tick_internal(time_frame);

		}// End while() udapte time

		}// End outer while() window->isOpen()


		

	} // End run()

void Application::tick_internal(float& time){

	this->tick(time);

}// End tick_internal()

void Application::tick(float& time){

	std::cout<<std::format("\t -> Frame rate at {:.2f} ",1.0f/time)<<std::endl;


}// End tick()


void Application::render(void){
	
	// create basic shape: rectangle
	sf::RectangleShape rect(sf::Vector2f({50.0f,50.0f}));

	rect.setFillColor(sf::Color::Red);

	// reshifting the origin
	rect.setOrigin(sf::Vector2f(50,50)); 

	// extracting window dimension
	sf::Vector2u window_dim = this->m_window.getSize();

	rect.setPosition(sf::Vector2f(window_dim.x/2,window_dim.y/2)) ;

	// drawing the rectangle
	this->m_window.draw(rect);
		

}// End render()


void Application::render_internal(void){

	// clear last frame	
	this->m_window.clear(sf::Color::Black);

	// render a shape
	this->render();

	// display new frame
	this->m_window.display();


}// End render_internal()



} // End namespace saga




