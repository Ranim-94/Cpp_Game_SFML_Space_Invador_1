

#pragma once

#include<SFML/Graphics.hpp>

namespace saga{

	class Application{

		private:

			sf::RenderWindow m_window;

			float m_frame_rate_target;

			sf::Clock m_tick_clock;

			void tick(float& time);

			void tick_internal(float& time);

			void render_internal(void);
			// render_internal() plays role of some base class

			virtual void render(void);
			// we use 'virtual' so we can override the render()
			// 




		public:

			Application();

			void run();



	}; // End class Application

} // End namespace saga


