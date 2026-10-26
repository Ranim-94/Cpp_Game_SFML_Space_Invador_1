

#pragma once

#include<memory>
#include<SFML/Graphics.hpp>

namespace saga{

	class World; // Declar class 'World'

	class Application{

		private:

			// declararing some sfml variables
			sf::RenderWindow m_window;

			float m_frame_rate_target;

			sf::Clock m_tick_clock;

			// App class manages the 'World'
			// we need 1st create a shared pointer
			std::shared_ptr<World> m_current_world;

			// ============ Some internal methods, specific to the engine ============
			void tick(float& time);

			void tick_internal(float& time);

			void render_internal(void);
			// render_internal() plays role of some base class

			virtual void render(void);
			// we use 'virtual' so we can override the render()
			
		public:

			Application();

			void run();

			
			
			template<typename WorldType>
			std::weak_ptr<WorldType> LoadWorld(){

			// creating a shared pointer of some type 'WorldType'
			std::shared_ptr<WorldType> new_world = 
			std::make_shared<WorldType>();

			/*
				Recall that when calling std::make_shared<type>(args),
			it will be calling some class constructor

			So in other words, a class will be instantiated
			
			*/	

			
			// Assiging 'm_current_world' pointer 
			// to App shared pointer new_world
			this->m_current_world = new_world;

			// this make sure that the 'Application' owns the 'World', and
			// not the other way around

			return new_world;

			} // End LoadWorld()
			
			

			


		}; // End class Application

} // End namespace saga


