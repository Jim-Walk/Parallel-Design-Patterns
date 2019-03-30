#ifndef SQUIRREL_HPP
#define SQUIRREL_HPP
#include <deque>

#include "../lib/squirrel-functions.h"
#include "actor.hpp"

class Squirrel : public Actor {
    public:
        Squirrel(float x, float y, Actor const& a) : Squirrel(a){
            pos_x = x;
            pos_y = y;
        }


        Squirrel(Actor const& a) : Actor(a){
            state = -1-id; 
            printf("my state is %ld\n", state);
            active = true;
            pos_x = 1;
            pos_y = 1;
            initialiseRNG(&state);
        }
        void run();
        void move();    
        void check_alive();
    private:
       float pos_x, pos_y; 
       float temp_x, temp_y; 
       long state;

       float cell_pop;
       float cell_inf;

       std::deque<float> pop_history;
       std::deque<float> inf_history;

       void give_birth(int cell);
       void die();

       void update_pop_history(float new_inf);
       void update_inf_history(float new_inf);

       bool get_infected();
       void set_infected(bool inf);

       bool get_alive();
       void set_alive(bool live);

       void step(int cell);
};

#endif
