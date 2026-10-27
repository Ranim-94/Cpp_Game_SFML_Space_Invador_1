

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

 namespace saga {

    class Application; // declare Application class

    class World{
    
        private:

            Application* m_owner_app;

            bool m_is_play;

        public:

            explicit World(Application* owner_app);

            // these are implemented in the engine
            void start_play_internal();

            void tick_internal(float time);

            // method will be override by the game
            virtual void start_play();
        
            virtual void tick(float time);
    
            virtual ~World();   

    } ; // End class World

 } // End saga
