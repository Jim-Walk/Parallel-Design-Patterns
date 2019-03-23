#include <mpi.h>
#include <thread>
#include <chrono>
#include "../include/master.hpp"
#include "../lib/pool.h"

void Master::set_up(){
    int cmd, workerRank;
    for (int i=0; i<num_grid_cells; i++){
        cmd = 1; 
        workerRank = startWorkerProcess();
        MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
        grid_ids.push_back(workerRank);
    }
    for (int i=0; i<total_squirrels; i++){
        cmd = 2; 
        workerRank = startWorkerProcess();
        MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
        squirrel_ids.push_back(workerRank);
    }
    active = true;
}

int Master::create_actor(int actor_type){
    actor_type += 1;
    int rank = startWorkerProcess();
    return rank;
}


bool Master::is_active(){
    return active;
}

void Master::run(){
    std::this_thread::sleep_for(std::chrono::seconds(2));
   
    if (live_squirrels == 0){
    // End the simulation by active changing to false
        active = false;
    }
}
