




#include "World.h"

#include "framework/Core.h"

#include "Actor.h"


namespace saga{

    World::World(Application* owner_app):
    m_owner_app{owner_app},
    m_is_play{false},
    m_Actor{},
    m_pending_Actor{}
    {

    }// End World()


    void World::start_play_internal(){

        if(!this->m_is_play){
        // Meaning the game didn't start yet    
            
            // start the game: switching to true
            this->m_is_play = true;

            LOG("Starting the game");

            this->start_play();

        } // End if() checking the game


    }// End start_play_internal()



    void World::tick_internal(float time){

        LOG("World Tick");

        // from the temporary list of Actors (whihc are 'm_pending_Actor'), 
        // create the list of actual Actors (which are 'm_Actor')
        for(std::shared_ptr<Actor> actor: this->m_pending_Actor){

            this->m_Actor.push_back(actor);

            actor->start_play_internal();

        } // End for 'm_pending_Actor'

        // clearing the temporary list of Actors, which are 'm_pending_Actor'
        this->m_pending_Actor.clear();

        // ietrating over the created 'm_Actor'
        for(std::shared_ptr<Actor> actor: this->m_Actor){

            actor->tick_internal(time);

        } // End for 'm_Actor'



        this->tick(time);


    }// End tick_internal()


    void World::tick(float time){
        
        // default empty implementation
        // To be implemented by the game

    } // End tick()

    void World::start_play(){
        // default empty implementation
        // To be implemented by the game


    }// End start_play()


    World::~World(){


    }


}// End namespace saga











