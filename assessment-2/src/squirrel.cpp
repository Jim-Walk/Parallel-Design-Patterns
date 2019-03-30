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
            pos_x = 1;
            pos_y = 1;
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
    // step into from current_cell
    step(current_cell);
    update_inf_history(cell_inf);
    float avg_inf = std::accumulate(inf_history.begin(), inf_history.end(), 0.0) / inf_history.size();
    if (!get_infected() ){
        if (willCatchDisease(avg_inf, &state)){
            set_infected(true);
            printf("%d: is infected!\n", id);
        }
    } else {
        if (willDie(&state)){
            die();
            return;
        } 
    }

    update_pop_history(cell_pop);
    float avg_pop = std::accumulate(pop_history.begin(), pop_history.end(), 0.0) / pop_history.size();
    //if (willGiveBirth(avg_pop, &state)){
        give_birth();
    //}
}

void Squirrel::step(int cell){
    if (get_infected()){
        send_msg(cell, MSG::STEP);
    } else {
        send_msg(cell, MSG::INFSTEP);
    }
    data_recv(&cell_pop);
    data_recv(&cell_inf);
}


void Squirrel::give_birth(){
    send_msg(0, MSG::START);
    float loc_vec[2] = {pos_x, pos_y};
    MPI_Bsend(loc_vec, 2, MPI_FLOAT, 0,0, MPI_COMM_WORLD);
    //printf("%d: I gave birth at %f, %f\n", id, pos_x, pos_y);
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

// tell master I have died, and die
void Squirrel::die(){
    send_msg(0, MSG::STOP);
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

void Squirrel::set_co_ords(){
    float loc_vec[2];
    MPI_Status stat;
    MPI_Recv(loc_vec, 2, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &stat);
    pos_x = loc_vec[0];
    pos_y = loc_vec[1];
}
