#include <thread>
#include <chrono>
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
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (i >= months){
            active = false;
        }
    }
    shutdownPool();
    printf("%d: clock shutdown pool\n", id);
    
}
