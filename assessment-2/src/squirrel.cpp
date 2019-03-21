#include "../lib/squirrel-functions.h"
#include "../include/squirrel.hpp"
#include "../lib/ran2.h"
#include <iostream>
#include <cstdlib>

void Squirrel::initialise(){
    state = 2;
    infected = false;
    active = true;
    alive = true;
    initialiseRNG(&state);
}

void Squirrel::run(){
    initialise();
    while (active){
        if (alive)
            move();
        check_active();
    }
}

void Squirrel::check_active(){
    std::cout << id << "sq: check if active" << std::endl; 
    active = false;
}

void Squirrel::move(){
    squirrelStep(pos_x, pos_y, &temp_x, &temp_y, &state);
    pos_x = temp_x;
    pos_y = temp_y;

    int current_cell = getCellFromPosition(pos_x, pos_y);
    // get info from current_cell
    float avg_inf_level = get_inf_level(current_cell);
    if (willCatchDisease(avg_inf_level, &state)){
        infected = true;
    }

    if (infected){
        infect_cell(current_cell);
        if (willDie(&state)){
            alive = false;
            return;
        }
    }

    float avg_pop = get_pop(current_cell);
    if (willGiveBirth(avg_pop, &state)){
        give_birth(pos_x, pos_y);
    }

    std::cout << id << ": I moved" << std::endl;
}

void Squirrel::infect_cell(int cell){
    std::cout << id << ": infecting " << cell << std::endl;
}

float Squirrel::get_inf_level(int cell){
    std::cout << id << ": getting level from " << cell << std::endl;
    return 2;
}

void Squirrel::give_birth(float x, float y){
    std::cout << id << ": I gave birth at " << x << ","<< y << std::endl;
}

float Squirrel::get_pop(int cell){
    std::cout << id << ": getting pop from " << cell << std::endl;
    return 2;
    
}
