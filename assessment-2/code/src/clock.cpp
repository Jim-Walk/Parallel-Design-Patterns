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

    int tick = 0;
    while (active){
        tick++;
        // Set up timiing
        auto tstart = std::chrono::system_clock::now();
        auto tend = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = tend - tstart;
        // check active for 800 milliseconds
        while (diff.count() < 0.50){
            tend = std::chrono::system_clock::now();
            diff = tend - tstart;
            check_active();
        }
        if (!active || tick >= months){
            break;
            active = false;
        }
        // Send tick to all grid cells and controller
        for (int rank = 1; rank <= 17; rank++){
            send_msg(rank, MSG::TICK);
        }
        printf("tick %d\n", tick);
        check_active();
    }
    if (tick >= months)
        shutdownPool();
    printf("%d: clock shutdown after %d months\n", id, tick);
}
