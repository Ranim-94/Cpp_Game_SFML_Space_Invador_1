

#include <iostream>
#include <SFML/Graphics.hpp>


int main() {

	// Instantiate a "window" object
	sf::RenderWindow window(sf::VideoMode({800,600}),
	"Space Invador");

	while(window.isOpen()){

		while(const std::optional window_event = window.pollEvent()){
		
			if(window_event->is<sf::Event::Closed>()){
				window.close();
			}
		
		
		} // End inner while()

	window.clear(sf::Color::Black);
	
	window.display();


	}// End outer while()


	return 0;

} // End main()
