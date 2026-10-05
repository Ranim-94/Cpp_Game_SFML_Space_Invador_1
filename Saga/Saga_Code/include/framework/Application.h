

#pragma once

#include<SFML/Graphics.hpp>

namespace saga{

	class Application{

		private:

			sf::RenderWindow m_window;

			float m_frame_rate_target;

			sf::Clock m_tick_clock;

			void tick(float& time);

			void render(void);




		public:

			Application();

			void run();



	}; // End class Application

} // End namespace saga


