

#include "framework/World.h"
#include "framework/Core.h"


namespace saga{

    World::World(Application* owner_app):
    m_owner_app{owner_app}{

    }// End World()


    void World::start_play_internal(){

        if(!this->m_is_play){
        // Meaning the game didn't start yet    
            
            // start the game: switching to true
            this->m_is_play = true;

            LOG("Starting the game");

            this->start_play();

        } // End if() checking the game


    }// End start_play()



    void World::tick_internal(float time){

        LOG("World Tick");

        this->tick(time);


    }// End tick()


    void World::tick(float time){
        
        // default empty implementation
        // To be implemented by the game

    } // End tick()

    void World::start_play(){
        // default empty implementation
        // To be implemented by the game


    }// End start_play()


}// End namespace saga






