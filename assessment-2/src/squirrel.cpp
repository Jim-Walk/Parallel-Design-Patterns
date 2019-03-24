#include <iostream>
#include <cstdlib>
#include <numeric>
#include <mpi.h>
#include "../lib/pool.h"
#include "../lib/squirrel-functions.h"
#include "../include/squirrel.hpp"
#include "../lib/ran2.h"

void Squirrel::initialise(){
}

void Squirrel::run(){
    printf("%d: start Squirrel\n", id);
    while (active){
        if (get_alive())
            move();
        check_active();
    }
}


void Squirrel::move(){
    squirrelStep(pos_x, pos_y, &temp_x, &temp_y, &state);
    pos_x = temp_x;
    pos_y = temp_y;

    int current_cell = getCellFromPosition(pos_x, pos_y);
    // get info from current_cell
    update_inf_history(current_cell);
    float avg_inf = std::accumulate(inf_history.begin(), inf_history.end(), 0.0) / inf_history.size();
    if (!get_infected() ){
        if (willCatchDisease(avg_inf, &state)){
            set_infected(true);
            printf("%d: is infected!\n", id);
        }
    } else {
        if (willDie(&state)){
            die();
            printf("%d: I died!\n", id);
            return;
        } 
    }

    update_pop_history(current_cell);
    float avg_pop = std::accumulate(pop_history.begin(), pop_history.end(), 0.0) / pop_history.size();
    if (willGiveBirth(avg_pop, &state)){
        give_birth(current_cell);
    }

}

//TODO Consider merging these functions so we only need
//     to send and recieve one message each step
float Squirrel::get_pop(int cell){
    float pop = 0;
    send_msg(cell, MSG::STEP);
    data_recv(&pop);
    return pop;
}

float Squirrel::get_inf_level(int cell){
    float inf = 0;

    if (!get_infected()){
        send_msg(cell, MSG::STEP);
    } else {
        send_msg(cell, MSG::INFSTEP);
    }
    data_recv(&inf);
    return inf;
}

void Squirrel::give_birth(int cell){
    std::cout << id << ": I gave birth at " << cell << std::endl;
}

void Squirrel::update_pop_history(int cell){
    float new_population = get_pop(cell);
    if (pop_history.size() > 49){
        pop_history.pop_back();
        pop_history.push_front(new_population);
    } else {
        pop_history.push_front(new_population);
    }
}

void Squirrel::update_inf_history(int cell){
    float new_inf = get_inf_level(cell);
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

