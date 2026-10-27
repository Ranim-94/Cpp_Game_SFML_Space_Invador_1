





#include "Actor.h"


namespace saga{




Actor::Actor(World* owner_world):
m_owner_world{owner_world},
m_is_play{false}{
   
}

Actor::~Actor() {
	
}

void Actor::start_play_internal(){

        if(!this->m_is_play){
        // Meaning the game didn't start yet    
            
            // start the game: switching to true
            this->m_is_play = true;

            LOG("Actor start play");

            this->start_play();

        } // End if() checking the game


    }// End start_play_internal()



    void Actor::tick_internal(float time){

        LOG("Actor Tick");

    }// End tick_internal()


	 void Actor::start_play(){
        // default empty implementation
        // To be implemented by the game


    }// End start_play()


} // End namespace saga
