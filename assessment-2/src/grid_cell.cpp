#include "../include/grid_cell.hpp"
#include <iostream>

void Grid_cell::run(){
    while (active){
        if (month_update){
            avg_pop = pop_count;
            pop_count = 0;
            avg_inf = inf_count/avg_pop;
            inf_count = 0;
            month_update = false;
        }
        check_for_month();
        exchange_pop();
        exchange_inf();
        check_active();
    }
}

void Grid_cell::check_for_month(){
    std::cout << id << ": checking if month has changed" << std::endl; 
    month_update = false;
}

void Grid_cell::exchange_pop(){
    std::cout << id << ": getting pop count and tell squirrl pop level" << std::endl;
}
void Grid_cell::exchange_inf(){
    std::cout << id << ": getting inf count and tell squirrl inf level" << std::endl;
}

void Grid_cell::check_active(){
    std::cout << id << ": checking if active" << std::endl;
    active = false;
}

