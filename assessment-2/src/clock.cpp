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
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (tick >= months){
            active = false;
        }
        //printf("sending to id:");
        // Send tick to master and all grid cells
        for (int rank = 0; rank <= 16; rank++){
          //  printf(" %d", rank);
            send_msg(rank, MSG::TICK);
        }
        //printf("\n");
        check_active();
    }
    printf("%d: clock shutdown after %d months\n", id, tick);
    shutdownPool();
    
}
