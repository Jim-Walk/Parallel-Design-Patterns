#include <iostream>
#include <cstdlib>
#include <numeric>
#include <mpi.h>
#include "../lib/squirrel-functions.h"
#include "../include/squirrel.hpp"
#include "../lib/ran2.h"

void Squirrel::initialise(){
    initialiseRNG(&state);
}

void Squirrel::run(){
    printf("%d: start Squirrel\n", id);
    initialise();
    while (active){
        if (alive)
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
    if (willCatchDisease(avg_inf, &state)){
        infected = true;
    }

    if (infected){
        infect_cell(current_cell);
        if (willDie(&state)){
            die();
            return;
        }
    }

    update_pop_history(current_cell);
    float avg_pop = std::accumulate(pop_history.begin(), pop_history.end(), 0.0) / pop_history.size();
    if (willGiveBirth(avg_pop, &state)){
        give_birth(current_cell);
    }

    //std::cout << id << ": I moved" << std::endl;
}

void Squirrel::infect_cell(int cell){
//    std::cout << id << ": infecting " << cell << std::endl;
}

float Squirrel::get_inf_level(int cell){
//    std::cout << id << ": getting level from " << cell << std::endl;
    return 2;
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

float Squirrel::get_pop(int cell){
    float pop = 0;
    send_msg(cell, MSG::STEP);
    data_recv(&pop);
    return pop;
}

void Squirrel::die(){
   // workerSleep();
    alive = false;
}
