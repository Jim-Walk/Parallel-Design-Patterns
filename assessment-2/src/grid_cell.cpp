#include <mpi.h>
#include <iostream>
#include "../include/grid_cell.hpp"
#include <tuple>

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
        exchange_pop();
        exchange_inf();
        check_active();
    }
    printf("gc %d: done, pop %f, inf %f\n", id, pop_count, inf_count);
}

void Grid_cell::check_for_month(){
    //std::cout << id << ": checking if month has changed" << std::endl; 
    month_update = false;
}

void Grid_cell::exchange_pop(){

    bool recvd;
    int rank = -1;
    std::tie(recvd, rank) = msg_recv();
    if (recvd){
        pop_count++;
        send_data(rank, avg_pop);
    }
}
void Grid_cell::exchange_inf(){
    bool recvd;
    int rank = -1;
    std::tie(recvd, rank) = msg_recv();
    if (recvd){
        inf_count++;
        send_data(rank, avg_pop);
    }
}

