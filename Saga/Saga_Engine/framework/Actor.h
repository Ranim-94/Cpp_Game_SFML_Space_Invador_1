

/*



*/


#pragma once

#include "framework/Core.h"

namespace saga{


class World; // forward declaration of class 'World' 


class Actor {


	private:

	World* m_owner_world;
	// why pointer of type World ?
	// this way the Actor can ask World to do 
	// something

	bool m_is_play;

	public:

		Actor(World* owner_world);

		virtual ~Actor();

		// these are implemented in the engine
		void start_play_internal();

		void tick_internal(float time);


		// method will be override by the game
        virtual void start_play();

		



	}; // End class 'Actor'

} // End namespace saga
