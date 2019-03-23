#ifndef SQUIRREL_HPP
#define SQUIRREL_HPP
#include <deque>

#include "actor.hpp"

class Squirrel : public Actor {
    public:
        Squirrel(Actor const& a) : Actor(a){
            state = 2;
            infected = false;
            active = true;
            alive = true;
            pos_x = 1;
            pos_y = 1;
        }
        void run();
        void move();    
        void initialise();
        float get_inf_level(int cell);
        void infect_cell(int cell);
    private:
       float pos_x, pos_y; 
       float temp_x, temp_y; 
       bool alive, infected;
       long state;

       std::deque<float> pop_history;
       std::deque<float> inf_history;

       void give_birth(int cell);
       void die();
       float get_pop(int cell);

       void update_pop_history(int cell);
       void update_inf_history(int cell);
};

#endif
