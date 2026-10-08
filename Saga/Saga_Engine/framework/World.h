


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

 #include "framework/Application.h"

 namespace saga {

    class World{

        public:

        explicit World(Application* owner_app);

        virtual void start_play();

        virtual void tick(float time);

        virtual ~World();


        private:

        Application* m_owner_app;

        bool m_is_play;

        void start_play_internal();

        void tick_internal(float time);




    } ; // 





 } // End saga
