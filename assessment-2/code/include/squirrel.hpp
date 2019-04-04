#ifndef SQUIRREL_HPP
#define SQUIRREL_HPP
#include <deque>

#include "../lib/squirrel-functions.h"
#include "actor.hpp"

class Squirrel : public Actor {
    public:

        Squirrel(Actor const& a);

        void run();
    private:
       float pos_x, pos_y; 
       float temp_x, temp_y; 
       long state;

       float cell_pop;
       float cell_inf;

       std::deque<float> pop_history;
       std::deque<float> inf_history;

       void move();    
       void give_birth();
       void die();

       void update_pop_history(float new_inf);
       void update_inf_history(float new_inf);

       bool get_infected();
       void set_infected(bool inf);

       bool get_alive();
       void set_alive(bool live);


       void set_co_ords();
       bool step(int cell);
};

#endif
