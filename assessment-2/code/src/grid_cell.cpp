#include <mpi.h>
#include <iostream>
#include "../include/grid_cell.hpp"
#include <tuple>
#include <limits>
#include <deque>
#include <numeric>

void Grid_cell::run(){
    std::deque<int> recent_pops;
    std::deque<int> recent_infs;
    int months = 0;
    while (active){
        // If It's been one month
        if (month_update){
            months++;

            // Update my population and infection history
            if (recent_pops.size() > 2){
                recent_pops.pop_back();
            }
            recent_pops.push_front(pop_count);
            if (recent_infs.size() > 1){
                recent_infs.pop_back();
            }
            recent_infs.push_front(inf_count);

            population_influx = std::accumulate(recent_pops.begin(), recent_pops.end(), 0);
            infection_level = std::accumulate(recent_infs.begin(), recent_infs.end(), 0);

            pop_count = 0; inf_count = 0;
            month_update = false;
            
            printf("gc %d: month: %d population_influx: %f infection_level: %f \n", id,months, population_influx, infection_level);
        }
        communicate();
        check_active();
    }
    printf("gc %d: DONE: pop: %f, inf: %f\n", id, population_influx, infection_level);
}

void Grid_cell::communicate(){

    bool recvd;
    int rank, msg  = -1;
    std::tie(recvd, rank, msg) = msg_recv();
    if (recvd){
        if (msg == MSG::STEP){
            pop_count++;
            send_data(rank, population_influx);
            send_data(rank, infection_level);
        } else if (msg == MSG::INFSTEP){
            pop_count++;
            inf_count++;
            send_data(rank, population_influx);
            send_data(rank, infection_level);
        } else if(msg == MSG::TICK){
            month_update=true;
        }
    }
}
