#include <iostream>
#include <cstdlib>
#include <numeric>
#include <mpi.h>
#include "../lib/pool.h"
#include "../lib/squirrel-functions.h"
#include "../include/squirrel.hpp"
#include "../lib/ran2.h"

Squirrel::Squirrel(Actor const& a) : Actor(a){
            state = -1-id; 
            active = true;
            initialiseRNG(&state);
            set_co_ords();
}

void Squirrel::run(){
    while (active){
        if (get_alive()){
            move();
        } else {
            break;
        }
        check_active();
    }
}


void Squirrel::move(){
    squirrelStep(pos_x, pos_y, &temp_x, &temp_y, &state);
    pos_x = temp_x;
    pos_y = temp_y;

    int current_cell = getCellFromPosition(pos_x, pos_y) +1;
    // If I can step into current_cell
    if (step(current_cell)){
        // Update my infection history
        // Get current infection level
        update_inf_history(cell_inf);
        float avg_inf = std::accumulate(
                            inf_history.begin(),
                            inf_history.end(),
                            0.0
                            ) / inf_history.size();
        // If I am not infected, check if I will catch the disease
        // and inform controller if I do
        if (!get_infected() ){
            if (willCatchDisease(avg_inf, &state)){
                set_infected(true);
                send_msg(17, MSG::INFSTEP);
            }
        } // If I am infected, check if I will die. If I die, exit
        else { // the function
            if (willDie(&state)){
                die();
                return;
            } 
        }
        // Update my population history
        // Get current population level
        update_pop_history(cell_pop);
        float avg_pop = std::accumulate(
                            pop_history.begin(), 
                            pop_history.end(), 
                            0.0
                            ) / pop_history.size();
        if (willGiveBirth(avg_pop, &state)){
            give_birth();
        }
    }
}

// Steps into cell, messages it with an infected or uninfected step
// Returns boolean value dependant on it recieving data from 
// target cell
bool Squirrel::step(int cell){
    if (!get_infected()){
        send_msg(cell, MSG::STEP);
    } else {
        send_msg(cell, MSG::INFSTEP);
    }
    return data_recv(cell, &cell_pop) &&  data_recv(cell, &cell_inf);
}

// Tells the controller that it is giving birth at a certain location
// TODO Fit this Bsend into actor send framework
void Squirrel::give_birth(){
    send_msg(17, MSG::START);
    float loc_vec[2] = {pos_x, pos_y};
    MPI_Bsend(loc_vec, 2, MPI_FLOAT, 17,0, MPI_COMM_WORLD);
}

void Squirrel::update_pop_history(float new_population){
    if (pop_history.size() > 49){
        pop_history.pop_back();
        pop_history.push_front(new_population);
    } else {
        pop_history.push_front(new_population);
    }
}

void Squirrel::update_inf_history(float new_inf){
    if (inf_history.size() > 49){
        inf_history.pop_back();
        inf_history.push_front(new_inf);
    } else {
        inf_history.push_front(new_inf);
    }
}

bool Squirrel::get_infected(){
    return act_type == actor_type::INFSQ;
}

void Squirrel::set_infected(bool inf){
    if (inf){
        act_type = actor_type::INFSQ;
    } else {
        act_type = actor_type::SQ;
    }
}

// Tell controller I have died, and die
void Squirrel::die(){
    send_msg(17, MSG::STOP);
    set_alive(false);
}

bool Squirrel::get_alive(){
    return act_type != actor_type::DEADSQ;
}

void Squirrel::set_alive(bool live){
    // all squirrels are born healthy
    if (live){
        act_type = actor_type::SQ;
    } else { //dead
        act_type = actor_type::DEADSQ;
    }
}
// Blocks untill squirrel recieves its birth location
// Squirrels must have an assigned location before they can move
void Squirrel::set_co_ords(){
    float loc_vec[2];
    MPI_Status stat;
    MPI_Recv(loc_vec, 2, MPI_FLOAT, parent, 0, MPI_COMM_WORLD, &stat);
    pos_x = loc_vec[0];
    pos_y = loc_vec[1];
}
