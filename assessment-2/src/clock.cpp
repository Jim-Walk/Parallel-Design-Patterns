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
        printf("Clock %d: Send tick %d \n", id, tick);
        for (int gc = 1; gc <= 16; gc++){
            send_msg(gc, MSG::TICK);
        }
        check_active();
    }
    printf("%d: clock shutdown after %d months\n", id, tick);
    shutdownPool();
    
}
