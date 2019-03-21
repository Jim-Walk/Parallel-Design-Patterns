#ifndef SQUIRREL_HPP
#define SQUIRREL_HPP
#include "actor.hpp"

class Squirrel : public Actor {
    public:
        Squirrel(Actor const& a) : Actor(a){
        }
        void run();
        void move();    
        void initialise();
        float get_inf_level(int cell);
        void infect_cell(int cell);
    private:
       float pos_x, pos_y; 
       float temp_x, temp_y; 
       bool active, alive, infected;
       long state;

       void give_birth(float x, float y);
       float get_pop(int cell);
       void check_active();

};

#endif
