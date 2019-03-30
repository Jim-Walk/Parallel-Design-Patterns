#include <mpi.h>
#include <iostream>
#include "../include/grid_cell.hpp"
#include <tuple>
#include <limits>

void Grid_cell::run(){
    //printf("%d: start grid cell\n", id);
    while (active){
        if (month_update){
            avg_pop = pop_count;
            pop_count = 0;
            avg_inf = inf_count/avg_pop;
            inf_count = 0;
            month_update = false;
        }
        check_for_month();
        exchange_pop_and_inf();
        check_active();
    }
    printf("gc %d: done, pop %f, inf %f\n", id, avg_pop, avg_inf);
}

void Grid_cell::check_for_month(){
    //std::cout << id << ": checking if month has changed" << std::endl; 
    month_update = false;
}

void Grid_cell::exchange_pop_and_inf(){

    bool recvd;
    int rank, msg  = -1;
    std::tie(recvd, rank, msg) = msg_recv();
    if (recvd){
        if (msg == MSG::STEP){
            pop_count++;
            send_data(rank, avg_pop);
            send_data(rank, avg_inf);
        } else if (msg == MSG::INFSTEP){
            pop_count++;
            inf_count++;
            send_data(rank, avg_pop);
            send_data(rank, avg_inf);
        } else if(msg == MSG::TICK){
            month_update=true;
        }
    }
}
