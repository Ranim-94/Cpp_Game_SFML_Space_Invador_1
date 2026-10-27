

/*
 *
 *
 *  A World (like in Unreal) manage multiple level in a game
 *
 *  Application class manage multiple world in the game
 *
 *  the game will include different type of worlds such as
 *  game level world, game menu, game success,
 *
 *
 * */

 #pragma once

 #include<memory>
 #include<list>

 namespace saga {

    class Application; // declare Application class

    class Actor;

    class World{
    
        private:

            Application* m_owner_app;

            bool m_is_play;

            std::list<std::shared_ptr<Actor>> m_Actor;

            std::list<std::shared_ptr<Actor>> m_pending_Actor;



        public:

            explicit World(Application* owner_app);

            // these are implemented in the engine
            void start_play_internal();
            void tick_internal(float time);

            // method will be override by the game
            virtual void start_play();        
            virtual void tick(float time);
    
            virtual ~World();   

            template<typename Actor_Type>
            std::weak_ptr<Actor_Type> Spawn_Actor(); 

    } ; // End class 'World'


    template<typename Actor_Type>
    std::weak_ptr<Actor_Type> World::Spawn_Actor(){

        std::shared_ptr<Actor_Type> new_actor_type = 
        std::make_shared<Actor_Type>(this);
        
        // pushing to the list
        this->m_pending_Actor.push_back(new_actor_type);

        return new_actor_type;

    }// End Spawn_Actor()



 } // End 'saga'
