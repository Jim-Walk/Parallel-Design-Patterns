#include "../lib/pool.h"
#include "../include/clock.hpp"

void Clock::set_months(int months){
    this->months = months;
}

int Clock::get_months(){
    return months;
}

void Clock::run(){

    int i = 0;
    while (active){
        i++;
        if (i >= months){
            active = false;
        }
    }
    shutdownPool();
    printf("%d clock shutdown pool\n", id);
    
}
